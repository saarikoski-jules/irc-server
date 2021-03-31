#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <string>

class Socket {
 public:
    int bind_things(int port);
    int open_connection(int fd);
    std::string* receive_data(int fd);
    void send_data(int fd, std::string msg);
 private:
    struct sockaddr_in addr;
};

class SocketException : public std::exception {
 public:
    SocketException(std::string const& what, const bool& fatal);
    ~SocketException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
};

#endif  //  SOCKET_H_
