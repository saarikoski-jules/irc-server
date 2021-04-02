/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:27:19 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/02 16:36:28 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "utils.h"

#include "logger.h"
#include "server_action.h"

Socket::Socket(ServerAction* serverAction) :
socketFd(-1),
addr(),
serverAction(serverAction) {
}

void Socket::bindAndListenToPort(const int& port) {
    struct sockaddr_in addr;
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    Utils::Mem::set(&addr, 0, sizeof(sockaddr));
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (bind(fd, (const sockaddr*)&addr, sizeof(addr)) != 0) {
        throw SocketException("Failed to bind socket", true);
    }
    // TODO(Jules): change backlog to appropriate value
    if (listen(fd, 1) != 0) {
        throw SocketException("Failed to listen to socket", true);
    }
    socketFd = fd;
}

void Socket::openConnection() {
    int addrlen;
    int clientFd;

    addrlen = sizeof(addr);
    fcntl(socketFd, F_SETFL, O_NONBLOCK);
    clientFd = accept(socketFd, reinterpret_cast<sockaddr*>(&addr),
        reinterpret_cast<socklen_t*>(&addrlen));
    if (clientFd >= 0) {
        Logger::log(LogLevelInfo, "Recieved a connection request");
        serverAction->type = ServerAction::NEW_CLIENT;
        serverAction->clientFd = clientFd;
    } else {
        throw SocketException("No connection request detected", false);
    }
}

void Socket::receiveData(const int& clientFd) {
    int chars_read;
    char data_buffer[512];

    Utils::Mem::set(data_buffer, 0, 512);
    // -2 in case of ending with a carriage return
    chars_read = read(clientFd, data_buffer, 512 - 2);
    // TODO(Jelle) See what happens when a message is longer than 512 bytes.
    if (chars_read > 0) {
        serverAction->type = ServerAction::NEW_MESSAGE;
        serverAction->clientFd = clientFd;
        serverAction->message = data_buffer;
        Logger::log(LogLevelDebug, "Received message from client:");
        Logger::log(LogLevelDebug, serverAction->message);
    } else {
        throw SocketException("No data received", false);
    }
}

void Socket::sendData(const int& clientFd, const std::string& msg) const {
    const char* cmsg = msg.c_str();
    const size_t msg_len = msg.length();

    if (send(clientFd, cmsg, msg_len, 0) <= 0) {
        throw SocketException("No data sent", false);
    }
}

SocketException::SocketException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
}

SocketException::~SocketException() throw() {
}

const bool& SocketException::isFatal() const {
    return (fatal);
}

const char* SocketException::what() const throw() {
    if (isFatal()) {
        return (std::string("Fatal socket exception: " + message).c_str());
    }
    return (std::string("Socket exception: " + message).c_str());
}
