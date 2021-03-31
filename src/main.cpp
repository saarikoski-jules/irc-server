/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:04:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 13:31:17 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include <iostream>//

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    Socket socket;
    int fd = socket.bind_things(8080);
    while(1) {
        // std::cout << "trying to open connection" << std::endl;
        int sockfd = socket.open_connection(fd);
        if (sockfd > 0) {
            std::cout << "received valid fd" << std::endl;
            while (1) {
                char* msg = socket.receive_data(sockfd);
                if (msg != NULL) {
                    std::cout << msg;
                    break ;
                    socket.send_data(sockfd, "msg received");
                }

            }
        }

    }

    return (0);
}
