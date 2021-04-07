/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:59:57 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/07 10:36:49 by jsaariko      ########   odam.nl         */
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
socket(&actions) {
    Logger::log(LogLevelInfo, "Attempting to create a server from port and password");
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

void Server::listenOnSocket() {
    try {
        socket.openConnection();
    } catch (const SocketException& e) {
        // Fall through because we got a normal message.
    }
    for (std::vector<Client>::iterator i = clients.begin();
    actions.size() == 0 && i != clients.end(); i++) {
        try {
            const Client& client = *i;
            socket.receiveData(client.fd);
        } catch (const SocketException& e) {
            // No message recieved.
        }
    }
}

void Server::handleAction() {
    while (!actions.empty()) {
        IServerAction* action = actions.front();
        action->execute(this);
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
