/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:59:57 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/01 10:31:57 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#include <string>
#include <cinttypes>
#include <exception>

#include "logger.h"
#include "socket.h"

Server::Server(const uint16_t port, std::string const& password) {
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

void Server::openSocket(const int& port) {
    Logger::log(LogLevelInfo, "Attempting to open socket");
    Socket socket;
    try {
        socket.bindAndListenToPort(port);
    } catch (const SocketException& e) {
        if (e.isFatal()) {
            Logger::log(LogLevelFatal, e.what());
            throw ServerException("Can't bind port", true);
        }
    }
    Logger::log(LogLevelInfo, "Waiting for incoming connection");

    int clientFd;
    std::string* msg;
    while (true) {
        try {
            clientFd = socket.openConnection();
            Logger::log(LogLevelInfo, "Accepted a connection request");
        } catch (const SocketException& e) {
            continue;
        }
        while (true) {
            try {
                msg = socket.receiveData(clientFd);
                Logger::log(LogLevelInfo, "Received msg");
                Logger::log(LogLevelInfo, (std::string const)*msg);
            } catch (const SocketException& e) {
                continue;
            }
        }
    }
}

void Server::validatePassword(std::string const& password) const {
    if (password == std::string("cats")) {
        Logger::log(LogLevelDebug, "Server password validated");
        return;
    }
    throw ServerException("Server password invalid", true);
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
