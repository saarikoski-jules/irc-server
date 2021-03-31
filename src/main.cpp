/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:04:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 15:15:31 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include <iostream>//

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    Socket socket;
    int fd;
    try {
        fd = socket.bind_things(8080);
    } catch (std::exception& e) {
        std::cout << e.what();
        return (1);
    }
    while(1) {
        int sockfd;
        try {
            sockfd = socket.open_connection(fd);
        } catch(std::exception& e) {
            continue;
        }
        std::cout << "received valid fd" << std::endl;
        while (1) {
            std::string* msg;
            try {
                msg = socket.receive_data(sockfd);
            } catch (std::exception& e) {
                continue;
            }
            std::cout << *msg;
            socket.send_data(sockfd, "msg received\n");
            break ;
        }
    }
    return (0);
}
