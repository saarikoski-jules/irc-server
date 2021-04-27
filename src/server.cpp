/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:59:57 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 18:12:46 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#include <string>
#include <chrono>
#include <thread>
#include <cinttypes>
#include <exception>

#include "logger.h"
#include "socket.h"
#include "connection.h"
#include "action_factory.h"

Server::Server(const uint16_t& port, const std::string& password) :
channels(),
serverSocket(&actions) {
    Logger::log(LogLevelInfo, "Attempting to create a server from port and password");
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
    Logger::log(LogLevelInfo, "Attempting to create a server from port and password");
    IServerAction::server = this;
    try {
        validatePassword(password);
        openSocket(port);
        startingServer->server.connectToServer(&startingServer->fd);
        sendAuthenticationTo(startingServer->fd, startingServer->server.password);
        connections.insert(std::pair<const int, Connection>(startingServer->fd, *startingServer));
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
            "SERVER irc.jelle 1 4242 :Codam development irc\r\n");
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Server::listenOnSocket() {
    try {
        serverSocket.checkNewConnections();
    } catch (const SocketException& e) {
        // Fall through because we got a normal message.
    }
    if (connections.empty() == false) {
        try {
            serverSocket.checkConnectionAndNewData(&connections);
        } catch (const SocketException& e) {
            // Fall through because we got a normal message.
        }
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

void Server::sendReplyToClient(const int& clientFd, const std::string& message) {
    // TODO(Jelle) Append the correct servername when it's available.
    serverSocket.sendData(clientFd, ":SERVERNAME " + message + "\r\n");
}

void Server::acceptNewConnection(const int& fd) {
    connections.insert(std::pair<const int, Connection>(fd, Connection(fd)));
}

void Server::deleteConnection(const int& fd) {
    std::map<const int, Connection>::iterator toDelete = connections.find(fd);
    if (toDelete != connections.end()) {
        connections.erase(toDelete);
    } else {
        Logger::log(LogLevelError, "Tried to delete non-existing connection");
    }
}

Connection* Server::getConnectionByFd(const int& fd) {
    return (&connections[fd]);
}

bool Server::nicknameExists(const std::string& nickName) {
    std::map<const int, Connection>::iterator it;
    for (it = connections.begin(); it != connections.end(); it++) {
        const Connection& connection = it->second;
        if (connection.connectionType == Connection::ClientType
        && connection.client.nickName == nickName) {
            return (true);
        }
    }
    return (false);
}

bool Server::usernameExists(const std::string& userName) {
    std::map<const int, Connection>::iterator it;
    for (it = connections.begin(); it != connections.end(); it++) {
        const Connection& connection = it->second;
        if (connection.connectionType == Connection::ClientType
        && connection.client.nickName == userName) {
            return (true);
        }
    }
    return (false);
}

Channel* Server::createNewChannel(const std::string& name, const int& fd) {
    Connection* chanop = getConnectionByFd(fd);
    Channel newChannel(name, chanop);
    channels.insert(std::pair<std::string, Channel>(name, newChannel));
    std::map<std::string, Channel>::iterator it = channels.find(name);
    return (&(*it).second);
}

Channel* Server::findChannel(const std::string& name) {
    std::map<std::string, Channel>::iterator it = channels.find(name);

    if (it != channels.end()) {
        return (&(*it).second);
    }
    throw std::out_of_range("Channel not found");
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
