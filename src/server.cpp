/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:59:57 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/09 13:22:26 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "server.h"

#include <string>
#include <chrono>
#include <thread>
#include <cinttypes>
#include <exception>
#include <stdexcept>  // For predetermined exception types
#include <ctime>

#include "logger.h"
#include "socket.h"
#include "connection.h"
#include "action_factory.h"
#include "construct_reply.h"
#include "int_conversions.h"

std::string Server::serverName = "lol";

Server::Server(const uint16_t& port, const std::string& password) :
channels(),
serverSocket(&actions) {
    Logger::log(LogLevelInfo, "Attempting to create a server from port and password");
    Server::serverName = std::string("irc." + IntConversion::intToString(port));
	time(&serverStart);
	IServerAction::server = this;
    try {
        validatePassword(password);
        openSocket(port);
    } catch (const ServerException& e) {
        if (e.isFatal()) {
            Logger::log(LogLevelFatal, e.what());
            throw ServerException("Not creating server", true);
        } else {
            Logger::log(LogLevelError, e.what());
        }
    }
}

Server::Server(Connection* startingServer, const uint16_t& port, const std::string& password) :
channels(),
serverSocket(&actions) {
    Server::serverName = std::string("irc." + IntConversion::intToString(port));
    Logger::log(LogLevelInfo, "Attempting to create a server from port and password");
	time(&serverStart);
    IServerAction::server = this;
    try {
        validatePassword(password);
        openSocket(port);
        startingServer->server.connectToServer(&startingServer->fd);
        sendAuthenticationTo(startingServer->fd, startingServer->server.password);
        connections.insert(std::pair<const int, Connection*>(startingServer->fd, startingServer));
        startingServer->isStartingServer = true;
    } catch (const ServerException& e) {
        if (e.isFatal()) {
            Logger::log(LogLevelFatal, e.what());
            throw ServerException("Not creating server", true);
        } else {
            Logger::log(LogLevelError, e.what());
        }
    } catch (const ServerConnectionException& e) {
        Logger::log(LogLevelFatal, e.what());
        throw ServerException("Not creating server", true);
    }
}

Server::~Server() {
    Logger::log(LogLevelInfo, "Server has been destructed");
}

void Server::validatePassword(std::string const& password) const {
    if (password == std::string("cats")) {
        Logger::log(LogLevelDebug, "Server password validated");
        return;
    }
    throw ServerException("Server password invalid", true);
}

void Server::openSocket(const uint16_t& port) {
    Logger::log(LogLevelInfo, "Attempting to open server socket");
    try {
        serverSocket.bindAndListenToPort(port);
    } catch (const SocketException& e) {
        if (e.isFatal()) {
            Logger::log(LogLevelFatal, e.what());
            throw ServerException("Can't open socket", true);
        }
    }
}

void Server::sendAuthenticationTo(const int& fd, const std::string& password) {
    try {
        actionFactory factory;
        std::vector<std::string> params;
        params.push_back("PASS " + password + " 0211 IRC|\r\n"
            "SERVER " + Server::serverName + " 1 4242 :Codam development irc\r\n");
        actions.push(factory.newAction("SEND", params, fd));
    } catch (const ActionFactoryException& e) {
        Logger::log(LogLevelError, e.what());
    }
}

void Server::run() {
    Logger::log(LogLevelInfo, "Starting up the server");
    while (true) {
        listenOnSocket();
        handleAction();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void Server::listenOnSocket() {
    serverSocket.createFdSet(&connections);
    int status = serverSocket.selectFdSet();
    if (status > 0) {
        try {
            serverSocket.checkNewConnections();
        } catch (const SocketException& e) {
            // Fall through because we got a normal message.
        }
        if (connections.empty() == false) {
            try {
                serverSocket.checkConnectionAndNewData();
            } catch (const SocketException& e) {
                // Fall through because we got a normal message.
            }
        }
    } else if (status == -1) {
        Logger::log(LogLevelError, "Select timed out in listenOnSocket");
    }
}

void Server::handleAction() {
    while (!actions.empty()) {
        IServerAction* action = actions.front();
        action->execute();
        delete action;
        actions.pop();
    }
    std::swap(actions, delayedActions);
}

void Server::sendMessage(const int& fd, const std::string& message) {
    try {
        serverSocket.sendData(fd, message);
    } catch (const SocketException& e) {
        Logger::log(LogLevelDebug, e.what());
        throw ServerException("Could not send message", false);
    }
}

void Server::sendMessageToServer(const int& fd, const std::string& message) {
    try {
        actionFactory factory;
        std::vector<std::string> replyVector;
        replyVector.push_back(message);
        addNewAction(factory.newAction("SEND", replyVector, fd));
    } catch (const std::exception& e) {
        Logger::log(LogLevelError, e.what());
    }
}

void Server::sendMessageToAllServers(const std::string& message) {
    sendMessageToAllServersButOne(message, -1);
}

void Server::sendMessageToAllServersButOne(const std::string& message, const int& exceptionFd) {
    try {
        actionFactory factory;
        std::vector<std::string> replyVector;
        replyVector.push_back(message);
        std::map<const int, Connection*>::iterator it = connections.begin();
        for (; it != connections.end(); it++) {
            const Connection* connection = it->second;
            if (connection->connectionType == Connection::ServerType && connection->fd != exceptionFd) {
                addNewAction(factory.newAction("SEND", replyVector, connection->fd));
            }
        }
    } catch (const std::exception& e) {
        Logger::log(LogLevelError, e.what());
    }
}

void Server::sendReplyToClient(const int& clientFd, const std::string& message, const std::string& prefix) {
    Logger::log(LogLevelDebug, "Messages going to be send to client.");
    Logger::log(LogLevelDebug, message);

    actionFactory factory;
	std::string replyString(":" + prefix + " " + message + "\r\n");
    std::vector<std::string> replyVector;
    replyVector.push_back(replyString);

    this->addNewAction(factory.newAction("SEND", replyVector, clientFd));
}

void Server::sendErrorToConnectionBypassingQueue(const int& fd, const std::string& message) {
    try {
        std::string fullMessage("ERROR :" + Server::serverName + " " + message + "\r\n");
        sendMessage(fd, fullMessage);
    } catch (const ServerException& e) {
        // Could not send message immediately. This is okay here.
    }
}

void Server::burstServerInformationTo(const int& fd) {
    Logger::log(LogLevelDebug, "Starting to burst to new server.");
    burstConnections(fd);
    burstChannels(fd);
}

void Server::burstConnections(const int& fd) {
    std::string reply;
    actionFactory factory;
    std::map<const int, Connection*>::iterator it = connections.begin();
    for (; it != connections.end(); it++) {
        Connection& connection = *it->second;
        if (connection.connectionType == Connection::ServerType && connection.fd != fd) {
            reply = constructNewServerBroadcast(connection);  // Sending unnecessary PASS message, is ignored.
            std::vector<std::string> replyVector;
            replyVector.push_back(reply);
            addNewAction(factory.newAction("SEND", replyVector, fd));
            burstLeafConnections(connection, fd);
        } else if (connection.connectionType == Connection::ClientType) {
            reply = constructNewNickBroadcast(connection);
            std::vector<std::string> replyVector;
            replyVector.push_back(reply);
            addNewAction(factory.newAction("SEND", replyVector, fd));
        }
    }
}

void Server::burstLeafConnections(Connection& connection, const int& fd) {
    std::string reply;
    actionFactory factory;
    std::vector<Connection*>::iterator it = connection.leafConnections.begin();
    for (; it != connection.leafConnections.end(); it++) {
        const Connection* leafConnection = *it;
        if (leafConnection->connectionType == Connection::ServerType) {
            reply = constructNewServerBroadcast(*leafConnection);  // Sending unnecessary PASS message, is ignored.
            std::vector<std::string> replyVector;
            replyVector.push_back(reply);
            addNewAction(factory.newAction("SEND", replyVector, fd));
        } else if (leafConnection->connectionType == Connection::ClientType) {
            reply = constructNewNickBroadcast(*leafConnection);
            std::vector<std::string> replyVector;
            replyVector.push_back(reply);
            addNewAction(factory.newAction("SEND", replyVector, fd));
        }
    }
}

void Server::burstChannels(const int& fd) {
    std::string reply;
    actionFactory factory;
    std::map<std::string, Channel*>::iterator it = channels.begin();
    for (; it != channels.end(); it++) {
        const Channel& channel = *it->second;
		if (channel.name[0] == '#') {
			std::vector<Connection*> connections = channel.getConnections();
			for (std::vector<Connection*>::iterator connectionIt = connections.begin(); connectionIt != connections.end(); connectionIt++) {
				const Connection* connection = *connectionIt;
				reply = constructJoinBroadcast(connection->client.nickName, channel.name);
				std::vector<std::string> replyVector;
				replyVector.push_back(reply);
				addNewAction(factory.newAction("SEND", replyVector, fd));
				replyVector.pop_back();
				if (channel.isOper(connection)) {
					reply = constructOperModeBroadcast(connection->client.nickName, channel.name);
				} else {
					reply = constructNoOperModeBroadcast(connection->client.nickName, channel.name);
				}
				replyVector.push_back(reply);
				addNewAction(factory.newAction("SEND", replyVector, fd));
				replyVector.pop_back();
			}
			std::vector<std::string> modeReplyVector;
			std::string channelMode = std::string(":" + Server::serverName + " MODE " + channel.getChannelModes());
			/* std::string channelMode = constructChannelModeIs(channel.name, channel->getModes(), modeParams); */
			modeReplyVector.push_back(channelMode);
			addNewAction(factory.newAction("SEND", modeReplyVector, fd));
		}
    }
}

void Server::acceptNewConnection(const int& fd) {
    connections.insert(std::pair<const int, Connection*>(fd, new Connection(fd)));
}

void Server::deleteConnection(const int& fd) {
    std::map<const int, Connection*>::iterator toDelete = connections.find(fd);
    if (toDelete != connections.end()) {
        delete toDelete->second;
        connections.erase(toDelete);
    } else {
        Logger::log(LogLevelError, "Tried to delete non-existing connection");
    }
}

Connection* Server::getConnectionByFd(const int& fd) {
    return (connections[fd]);
}

Connection* Server::getClientByNick(const std::string& nick) {
    std::map<const int, Connection*>::iterator it = connections.begin();
    for (; it != connections.end(); it++) {
	    Connection* connection = it->second;
        if (connection->connectionType == Connection::ClientType) {
            if (connection->client.nickName == nick) {
                return (it->second);
            }
        } else if (connection->connectionType == Connection::ServerType) {
            try {
                Connection* c = connection->getLeafConnection(nick);
                return (c);
            } catch (const std::out_of_range& e) {
                // Nickname not present in this leaf connection.
            }
        }
    }
    throw std::invalid_argument("Could not find the nick in list of clients");
}

bool Server::hasLocalConnection(const Connection& connection) {
    const Connection* otherConnection = getConnectionByFd(connection.fd);
    if (connection.connectionType != otherConnection->connectionType) {
        return (false);
    } else if (connection.connectionType == Connection::ClientType) {
        if (connection.client.nickName != otherConnection->client.nickName) {
            return (false);
        }
    } else if (connection.connectionType == Connection::ServerType) {
        if (connection.server.name != otherConnection->server.name) {
            return (false);
        }
    }
    return (true);
}

bool Server::fdExists(const int& fd) {
    std::map<const int, Connection*>::iterator it = connections.find(fd);
    if (it == connections.end()) {
        return (false);
    }
    return (true);
}

bool Server::nicknameExists(const std::string& nickName) {
    try {
        Connection *dummy = getClientByNick(nickName);
        (void)dummy;
        return (true);
    } catch (const std::invalid_argument& e) {
        return (false);
    }
}

bool Server::usernameExists(const std::string& userName) {
    std::map<const int, Connection*>::iterator it;
    for (it = connections.begin(); it != connections.end(); it++) {
        const Connection& connection = *(it->second);
        if (connection.connectionType == Connection::ClientType
        && connection.client.nickName == userName) {
            return (true);
        }
    }
    return (false);
}

bool Server::servernameExists(const std::string& serverName) {
    if (serverName == Server::serverName) {
        return (true);
    }
    std::map<const int, Connection*>::iterator it = connections.begin();
    for (; it != connections.end(); it++) {
	    Connection* connection = it->second;
        if (connection->connectionType == Connection::ServerType) {
            if (connection->server.name == serverName) {
                return (true);
            } else {
                std::vector<Connection*>::iterator leafIt = connection->leafConnections.begin();
                for (; leafIt != connection->leafConnections.end(); leafIt++) {
	                Connection* leafConnection = *leafIt;
                    if (leafConnection->connectionType == Connection::ServerType
                    && leafConnection->server.name == serverName) {
                        return (true);
                    }
                }
            }
        }
    }
    return (false);
}

bool Server::serverTokenExists(const std::string& serverToken) {
    std::map<const int, Connection*>::iterator it;
    for (it = connections.begin(); it != connections.end(); it++) {
        const Connection& connection = *(it->second);
        if (connection.connectionType == Connection::ServerType
        && connection.server.token == serverToken) {
            return (true);
        }
    }
    return (false);
}

Channel* Server::createNewChannel(const std::string& name, Connection* chanop) {
    // Connection* chanop = getConnectionByFd(fd);
    Channel *newChannel = new Channel(name, chanop);
    channels.insert(std::pair<std::string, Channel*>(name, newChannel));
    // std::map<std::string, Channel*>::iterator it = channels.find(name);
    return (channels[name]);
    // return (&(*it).second);
}

Channel* Server::findChannel(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = channels.find(name);

    if (it != channels.end()) {
        return ((*it).second);
    }
    throw std::out_of_range("Channel not found");
}

void Server::sendMessageToAllLocalUsersInClientChannels(
	const Connection* connection, const std::string& message, const std::string& prefix) {
	for (std::map<std::string, Channel*>::iterator i = channels.begin(); i != channels.end(); i++) {
		if ((*i).second->connectionIsInChannel(connection)) {
            std::vector<Connection*> sendTo = (*i).second->getConnections();
            for (std::vector<Connection*>::iterator j = sendTo.begin(); j != sendTo.end(); j++) {
				if (hasLocalConnection(**j) && connection != (*j)) {
                    sendReplyToClient((*j)->fd, message, prefix);
                }
            }
        }
    }
}

void Server::removeClientFromChannels(const Connection* con) {
    for (std::map<std::string, Channel*>::iterator i = channels.begin(); i != channels.end();) {
        Channel *chan = i->second;
        try {
			chan->removeConnection(con);
			if (chan->getAmtUsers() == 0) {
				delete chan;
				i = channels.erase(i);
			} else {
				i++;
			}
		} catch (const std::exception& e) {
			i++;
		}
    }
}

void Server::deleteChannel(Channel* chan) {
	std::map<std::string, Channel*>::iterator chanPos = channels.find(chan->name);
	channels.erase(chanPos);
	delete chan;
}

std::map<std::string, Channel*> Server::getListOfChannels() {
    return (channels);
}

void Server::delayFirstAction() {
    delayedActions.push(actions.front()->clone());
}

void Server::addNewAction(IServerAction* action) {
    actions.push(action);
}

ServerException::ServerException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
    if (isFatal()) {
        fullMessage = std::string("Fatal server exception: " + message);
    } else {
        fullMessage = std::string("Server exception: " + message);
    }
}

ServerException::~ServerException() throw() {
}

const bool& ServerException::isFatal() const {
    return (fatal);
}

const char* ServerException::what() const throw() {
    return (fullMessage.c_str());
}
