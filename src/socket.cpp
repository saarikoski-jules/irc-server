/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:27:19 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 17:46:52 by jsaariko      ########   odam.nl         */
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

Socket::Socket(std::queue<IServerAction*>* actions) :
socketFd(-1),
addr(),
actions(actions) {
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
    IServerAction* action;

    addrlen = sizeof(addr);
    fcntl(socketFd, F_SETFL, O_NONBLOCK);
    clientFd = accept(socketFd, reinterpret_cast<sockaddr*>(&addr),
        reinterpret_cast<socklen_t*>(&addrlen));
    if (clientFd >= 0) {
        fcntl(clientFd, F_SETFL, O_NONBLOCK);
        std::vector<std::string> vec;
        action = new ServerActionAccept(vec, clientFd);
        actions->push(action);

        Logger::log(LogLevelInfo, "Recieved a connection request");
    } else {
        throw SocketException("No connection request detected", false);
    }
}

void Socket::receiveData(const int& clientFd) {
    int chars_read;
    char data_buffer[MAX_MESSAGE_SIZE + 1];
    IServerAction *action;

    Utils::Mem::set(data_buffer, 0, MAX_MESSAGE_SIZE + 1);
    chars_read = read(clientFd, data_buffer, MAX_MESSAGE_SIZE);
    // TODO(Jelle) See what happens when a message is longer than 512 bytes.
    std::vector<std::string> vec;
    Logger::log(LogLevelDebug, "receive data");
    if (chars_read > 0) {
        vec.push_back(data_buffer);
        action = new ServerActionReceive(vec, clientFd);
        actions->push(action);

        Logger::log(LogLevelDebug, "Received message from client:");
        Logger::log(LogLevelDebug, data_buffer);
    } else if (chars_read == 0) {
        action = new ServerActionDisconnect(vec, clientFd);
        actions->push(action);

        Logger::log(LogLevelDebug, "read 0 chars, disconnecting client");
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
