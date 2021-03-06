/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                        :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:27:19 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/09 15:13:46 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <map>

#include "logger.h"
#include "iserver_action.h"
#include "utils.h"
#include "connection.h"
#include "server_connection.h"
#include "server_action_accept.h"
#include "server_action_disconnect.h"
#include "server_action_receive.h"

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
    if (listen(fd, 1) != 0) {
        throw SocketException("Failed to listen to socket", true);
    }
    socketFd = fd;
    fcntl(socketFd, F_SETFL, O_NONBLOCK);
}

void Socket::createFdSet(std::map<const int, Connection*>* connections) {
    fd_set fdSet;
    FD_ZERO(&fdSet);
    maxFd = socketFd;

    FD_SET(socketFd, &fdSet);
    std::map<const int, Connection*>::iterator it;
    for (it = connections->begin(); it != connections->end(); it++) {
        const Connection* connection = it->second;
        const int& fd = connection->fd;
        FD_SET(fd, &fdSet);
        if (fd > maxFd) {
            maxFd = fd;
        }
    }
    readSet = fdSet;
    writeSet = fdSet;
}

int Socket::selectFdSet() {
    struct timeval waitFor;
    waitFor.tv_sec = 0;
    waitFor.tv_usec = 100000;
    return (select(maxFd + 1, &readSet, &writeSet, NULL, &waitFor));
}

void Socket::checkNewConnections() {
    if (FD_ISSET(socketFd, &readSet)) {
        int addrlen;
        int fd;

        addrlen = sizeof(addr);
        fd = accept(socketFd, reinterpret_cast<sockaddr*>(&addr),
            reinterpret_cast<socklen_t*>(&addrlen));
        if (fd >= 0) {
            Logger::log(LogLevelInfo, "Recieved a connection request");
            fcntl(fd, F_SETFL, O_NONBLOCK);
            std::vector<std::string> vec;
			IServerAction* action = new ServerActionAccept(vec, fd, "");
            actions->push(action);
        } else {
            throw SocketException("No connection request detected", false);
        }
    }
}

void Socket::checkConnectionAndNewData() {
    readFromFds();
}

void Socket::readFromFds() {
    int chars_read;
    char data_buffer[MAX_MESSAGE_SIZE + 1];
    IServerAction *action;

    for (int fd = 0; fd <= maxFd; fd++) {
        if (fd != socketFd && FD_ISSET(fd, &readSet)) {
            Utils::Mem::set(data_buffer, 0, MAX_MESSAGE_SIZE + 1);
            chars_read = read(fd, data_buffer, MAX_MESSAGE_SIZE);
            if (chars_read > 0) {
                std::vector<std::string> vec;
                vec.push_back(data_buffer);
				action = new ServerActionReceive(vec, fd, "");
                actions->push(action);

                Logger::log(LogLevelDebug, "Received message from client:");
                Logger::log(LogLevelDebug, data_buffer);
            } else {
                std::vector<std::string> vec;
				action = new ServerActionDisconnect(vec, fd, "");
                actions->push(action);
            }
        }
    }
}

void Socket::sendData(const int& fd, const std::string& msg) {
    if (FD_ISSET(fd, &writeSet)) {
        FD_CLR(fd, &writeSet);
        if (send(fd, msg.c_str(), msg.length(), 0) <= 0) {
            throw SocketException("No data sent", false);
        }
    } else {
        throw SocketException("Fd not available for writing", false);
    }
}

SocketException::SocketException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
    if (isFatal()) {
        fullMessage = std::string("Fatal socket exception: " + message);
    } else {
        fullMessage = std::string("Socket exception: " + message);
    }
}

SocketException::~SocketException() throw() {
}

const bool& SocketException::isFatal() const {
    return (fatal);
}

const char* SocketException::what() const throw() {
    return (fullMessage.c_str());
}
