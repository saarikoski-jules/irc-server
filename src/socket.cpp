/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:27:19 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/03/31 14:33:50 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include <iostream>//
#include "utils.hpp"

int Socket::bind_things(int port) {
    struct sockaddr_in addr;

    Utils::Mem::set(&addr, 0, sizeof(sockaddr));
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int ret = bind(fd, (const sockaddr*)&addr, sizeof(addr));
    if (ret != 0) {
        std::cout << "fuck bind" << std::endl;
        return (-1);
    } else {
        std::cout << "bind" << std::endl;
    }
    ret = listen(fd, 1);//TODO: (Jules)change backlog to appropriate value
    if (ret != 0) {
        std::cout << "fuck listen" << std::endl;
        return (-1);
    } else {
        std::cout << "listening succesfully" << std::endl;
    }
    return (fd);

}

int Socket::open_connection(int fd) {

    int addrlen = sizeof(addr);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    int sockfd2 = accept(fd, (sockaddr *)&addr, (socklen_t *)&addrlen);
    if (sockfd2 <= 0) {
        return (-1);
    }
    return (sockfd2);
}

char* Socket::receive_data(int sockfd) {
    char* data_buffer = new char[1024];
    Utils::Mem::set(data_buffer, 0, 1024);

    int chars_read = read(sockfd, data_buffer, 1024);
    if (chars_read > 0) {
        return (data_buffer);
    } else {
        delete[] data_buffer;
        return (NULL);
    }
}

void Socket::send_data(int sockfd, std::string msg) {
    std::cout << "here yall" << std::endl;
    const char* cmsg = msg.c_str();
    size_t msg_len = msg.length();

    send(sockfd, cmsg, msg_len, 0);
    std::cout << "sending" << std::endl;
}

