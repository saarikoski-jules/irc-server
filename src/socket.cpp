/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:27:19 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/10 12:52:13 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include "utils.h"

#include "logger.h"
#include "server_action.h"
#include "action_factory.h"

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
    fcntl(socketFd, F_SETFL, O_NONBLOCK);
}

void Socket::checkNewConnections() {
    int addrlen;
    int clientFd;
    IServerAction* action;

    addrlen = sizeof(addr);
    clientFd = accept(socketFd, reinterpret_cast<sockaddr*>(&addr),
        reinterpret_cast<socklen_t*>(&addrlen));
    if (clientFd >= 0) {
        actionFactory factory;
        fcntl(clientFd, F_SETFL, O_NONBLOCK);
        std::vector<std::string> vec;
        action = factory.newAction("ACCEPT", vec, clientFd);
        actions->push(action);
        Logger::log(LogLevelInfo, "Recieved a connection request");
    } else {
        throw SocketException("No connection request detected", false);
    }
}

int Socket::createFdSet(const std::vector<Client>& clients, fd_set* set) {
    fd_set fdSet;
    FD_ZERO(&fdSet);
    int maxFd = 0;

    for (std::vector<const Client>::iterator i = clients.begin(); i != clients.end(); i++) {
        FD_SET((*i).fd, &fdSet);
        if ((*i).fd > maxFd) {
            maxFd = (*i).fd;
        }
    }
    *set = fdSet;
    return (maxFd);
}

void Socket::readFromFds(const std::vector<Client>& clients, fd_set readSet) {
    int chars_read;
    char data_buffer[MAX_MESSAGE_SIZE + 1];
    IServerAction *action;
    actionFactory factory;

    for (std::vector<const Client>::iterator i = clients.begin(); i != clients.end(); i++) {
        Utils::Mem::set(data_buffer, 0, MAX_MESSAGE_SIZE + 1);
        if (FD_ISSET((*i).fd, &readSet)) {
            chars_read = read((*i).fd, data_buffer, MAX_MESSAGE_SIZE);
            std::vector<std::string> vec;
            if (chars_read > 0) {
                vec.push_back(data_buffer);
                action = factory.newAction("RECEIVE", vec, (*i).fd);
                actions->push(action);

                Logger::log(LogLevelDebug, "Received message from client:");
                Logger::log(LogLevelDebug, data_buffer);
            } else {
                action = factory.newAction("DISCONNECT", vec, (*i).fd);
                actions->push(action);
            }
        }
    }
    // TODO(Jelle) See what happens when a message is longer than 512 bytes.
}

void Socket::checkConnectionAndNewDataFrom(const std::vector<Client>& clients) {
    struct timeval waitFor;
    fd_set readSet;

    waitFor.tv_sec = 0;
    waitFor.tv_usec = 100000;

    int maxFd = createFdSet(clients, &readSet);
    int status = select(maxFd + 1, &readSet, NULL, NULL, &waitFor);

    if (status != 0) {
        readFromFds(clients, readSet);
    } else {
        throw SocketException("Select timeout", false);
    }
}

void Socket::sendData(const int& clientFd, const std::string& msg) const {
    const char* cmsg = msg.c_str();
    const size_t msg_len = msg.length();
    struct timeval waitFor;
    fd_set writeSet;

    FD_ZERO(&writeSet);
    FD_SET(clientFd, &writeSet);
    waitFor.tv_sec = 0;
    waitFor.tv_usec = 0;

    if (select(clientFd + 1, NULL, &writeSet, NULL, &waitFor) > 0) {
        if (send(clientFd, cmsg, msg_len, 0) <= 0) {
            throw SocketException("No data sent", false);
        }
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
