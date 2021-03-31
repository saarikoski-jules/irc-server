/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:27:19 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/03/31 15:39:29 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include "utils.h"

int Socket::bind_things(int port) {
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
    return (fd);
}

int Socket::open_connection(int fd) {
    int addrlen;
    int sockfd2;

    addrlen = sizeof(addr);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    sockfd2 = accept(fd, reinterpret_cast<sockaddr*>(&addr),
        reinterpret_cast<socklen_t*>(&addrlen));
    if (sockfd2 <= 0) {
        throw SocketException("No connection request detected", false);
    }
    return (sockfd2);
}

std::string* Socket::receive_data(int sockfd) {
    char* data_buffer;
    int chars_read;

    data_buffer = new char[1024];
    Utils::Mem::set(data_buffer, 0, 1024);
    chars_read = read(sockfd, data_buffer, 1024);

    if (chars_read > 0) {
        std::string* data = new std::string(data_buffer);
        delete[] data_buffer;
        return (data);
    } else {
        delete[] data_buffer;
        throw SocketException("No data received", false);
    }
}

void Socket::send_data(int sockfd, std::string msg) {
    const char* cmsg = msg.c_str();
    const size_t msg_len = msg.length();

    if (send(sockfd, cmsg, msg_len, 0) <= 0) {
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
        return (std::string("Fatal argument exception: " + message).c_str());
    }
    return (std::string("Argument exception: " + message).c_str());
}
