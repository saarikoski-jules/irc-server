#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


class Socket {
 public:
    int bind_things(int port);
    int open_connection(int fd);
    char* receive_data(int fd);
    void send_data(int fd, std::string msg);
 private:
    struct sockaddr_in addr;

};

#endif  //  SOCKET_HPP_
