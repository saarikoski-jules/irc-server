/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:27:19 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/03/31 13:32:47 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include <iostream>//


void    *ft_memset(void *ptr, int c, size_t n) {
	unsigned char *ptr_copy;

	ptr_copy = static_cast<unsigned char *>(ptr);
	while (n > 0)
	{
		*ptr_copy = c;
		ptr_copy++;
		n--;
	}
	return (static_cast<void *>(ptr));
}

int Socket::bind_things(int port) {
    struct sockaddr_in addr;

    ft_memset(&addr, 0, sizeof(sockaddr));
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    int opt = 1;

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));


    int ret = bind(fd, (const sockaddr*)&addr, sizeof(addr));
    if (ret != 0) {
        std::cout << "fuck bind" << std::endl;
        return (-1);
    } else {
        std::cout << "bind" << std::endl;
    }
    ret = listen(fd, 1);//TODO: change backlog to appropriate value
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
    // std::cout << "accept" << sockfd2 << std::endl;
    if (sockfd2 <= 0) {
        // std::cout << "fuck accept" << std::endl;
        return (-1);
    } else {
        std::cout << "accept" << std::endl;
    }
    return (sockfd2);
}

char* Socket::receive_data(int sockfd) {
    char* data_buffer = new char[1024];
    ft_memset(data_buffer, 0, 1024);

    int chars_read = read(sockfd, data_buffer, 1024);
    if (chars_read > 0) {
        return (data_buffer);
    } else {
        delete[] data_buffer;
        return (NULL);
    }
}

void Socket::send_data(int sockfd, std::string msg) {
    
    send(sockfd, msg, 1, 0);//TODO: change
    std::cout << "sending" << std::endl;
}

