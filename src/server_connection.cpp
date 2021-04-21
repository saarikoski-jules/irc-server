/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_connection.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 14:04:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/21 18:19:06 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_connection.h"

#include <string>
#include <vector>
#include <exception>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"
#include "logger.h"
#include "string_conversions.h"

ServerConnection::ServerConnection() :
hostName(""),
password(""),
port(UINT16_MAX) {
}

// Configuration line expects "host:port:password"
ServerConnection::ServerConnection(const std::string& configuration) {
    try {
        std::vector<std::string> config = Utils::String::tokenize(configuration, configuration.length(), ":");
        if (config.size() == 3) {
            hostName = config[0];
            port = StringConversion::toUint16(config[1]);
            password = config[2];
        } else {
            throw ServerConnectionException("Invalid configuration string passed", true);
        }
    } catch (const std::out_of_range& e) {
        Logger::log(LogLevelError, e.what());
        throw ServerConnectionException("Invalid configuration string passed", true);
    }
}

void ServerConnection::connectToServer(int* fd) {
    struct sockaddr_in serverAddress;

    *fd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(*fd, F_SETFL, O_NONBLOCK);
    Utils::Mem::set(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(hostName.c_str());
    serverAddress.sin_port = htons(port);
    if (connect(*fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        Logger::log(LogLevelDebug, "Remote server failed to connect");
        throw ServerConnectionException("Could not connect to remote server", true);
    }
    Logger::log(LogLevelInfo, "Connected remote server succesfully");
}

ServerConnectionException::ServerConnectionException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
}

ServerConnectionException::~ServerConnectionException() throw() {
}

const bool& ServerConnectionException::isFatal() const {
    return (fatal);
}

const char* ServerConnectionException::what() const throw() {
    if (isFatal()) {
        return (std::string("Fatal server connection exception: " + message).c_str());
    }
    return (std::string("Server connection exception: " + message).c_str());
}
