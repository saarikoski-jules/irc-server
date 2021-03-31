#include "socket.hpp"
#include <iostream>//

#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>//

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

int Socket::open() {

    struct sockaddr_in addr;
    char data_buffer[1024];

    ft_memset(&data_buffer, 0, 1024);
    ft_memset(&addr, 0, sizeof(sockaddr));
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    std::cout << "data buffer '" << data_buffer << "'" << std::endl;

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    std::cout << "defined addr" << std::endl;

    int opt = 1;

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));


    int ret = bind(fd, (const sockaddr*)&addr, sizeof(addr));
    if (ret != 0) {
        std::cout << "fuck bind" << std::endl;
    } else {
        std::cout << "bind" << std::endl;
    }

    ret = listen(fd, 1);//TODO: change backlog to appropriate value
    if (ret != 0) {
        std::cout << "fuck listen" << std::endl;
    } else {
        std::cout << "listening succesfully" << std::endl;
    }
    int addrlen = sizeof(addr);
    int sockfd2 = accept(fd, (sockaddr *)&addr, (socklen_t *)&addrlen);
    if (sockfd2 <= 0) {
        std::cout << "fuck accept" << std::endl;
    } else {
        std::cout << "accept" << std::endl;
    }

    int chars_read = read(sockfd2, &data_buffer, 1024);
    if (chars_read > 0) {
        std::cout << "read " << chars_read << std::endl;
        std::cout << data_buffer << std::endl;
    }
    std::cout << "reading" << std::endl;

    send(sockfd2, "aaa", 3, 0);
    std::cout << "sending" << std::endl;

    return (1);
}
