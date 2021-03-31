#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


class Socket {
 public:
    int bind_things(int);
    int open_connection(int);
    char* receive_data(int);
    void send_data(int, char*);
 private:
    struct sockaddr_in addr;

};

#endif  //  SOCKET_HPP_
