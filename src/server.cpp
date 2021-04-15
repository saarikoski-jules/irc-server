/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:59:57 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/15 11:41:39 by jsaariko      ########   odam.nl         */
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

Server::Server(const uint16_t& port, const std::string& password) :
clients(),
channels(),
socket(&actions) {
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
        socket.bindAndListenToPort(port);
    } catch (const SocketException& e) {
        if (e.isFatal()) {
            Logger::log(LogLevelFatal, e.what());
            throw ServerException("Can't open socket", true);
        }
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

void Server::sendReplyToClient(const int& clientFd, const std::string& message) {
    // TODO(Jelle) Append the correct servername when it's available.
    socket.sendData(clientFd, ":SERVERNAME " + message + "\r\n");
}

void Server::listenOnSocket() {
    try {
        socket.checkNewConnections();
    } catch (const SocketException& e) {
        // Fall through because we got a normal message.
    }
    if (!clients.empty()) {
        try {
            socket.checkConnectionAndNewDataFrom(&clients);
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
}

void Server::acceptNewClient(const int& clientFd) {
    clients.push_back(Client(clientFd));
}

void Server::deleteClient(const int& clientFd) {
    std::vector<Client>::iterator it = clients.begin();
    while (it != clients.end()) {
        if ((*it).fd == clientFd) {
            Logger::log(LogLevelInfo, "Client disconnected");
            clients.erase(it);
            break;
        }
        it++;
    }
}

Client* Server::getClientByFd(const int& clientFd) {
    std::vector<Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++) {
        const Client& client = *it;
        if (client.fd == clientFd) {
            return &(*it);
        }
    }
    throw std::invalid_argument("Could not find the clientFd in list of clients");
}

Client* Server::getClientByNick(const std::string& nick) {
    std::vector<Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++) {
        const Client& client = *it;
        if (client.nickName == nick) {
            return &(*it);
        }
    }
    throw std::invalid_argument("Could not find the nick in list of clients");
}

bool Server::nicknameExists(const std::string& nickName) {
    std::vector<Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++) {
        const Client& client = *it;
        if (client.nickName == nickName) {
            return (true);
        }
    }
    return (false);
}

bool Server::usernameExists(const std::string& userName) {
    std::vector<Client>::iterator it = clients.begin();
    for (; it != clients.end(); it++) {
        const Client& client = *it;
        if (client.userName == userName) {
            return (true);
        }
    }
    return (false);
}

Channel* Server::createNewChannel(const std::string& name, const int& clientFd) {
    Client* chanop = getClientByFd(clientFd);
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

void Server::addNewAction(IServerAction* action) {
    actions.push(action);
}

ServerException::ServerException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
}

ServerException::~ServerException() throw() {
}

const bool& ServerException::isFatal() const {
    return (fatal);
}

const char* ServerException::what() const throw() {
    if (isFatal()) {
        return (std::string("Fatal server exception: " + message).c_str());
    }
    return (std::string("Server exception: " + message).c_str());
}
