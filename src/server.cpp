/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:59:57 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 13:03:54 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

#include "logger.h"

Server::Server(const uint16_t port, std::string const& password) {
    (void)port;
    Logger::log(LogLevelInfo, "Attempting to create a server from port and password");
    try {
        validatePassword(password);
        // TODO(Jelle) Open server socket.
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
