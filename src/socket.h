/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:48:54 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/03/31 16:41:37 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <string>

class Socket {
 public:
    void bindAndListenToPort(const int& port);
    int openConnection();
    std::string* receiveData(int clientFd);
    void send_data(int fd, std::string msg);
 private:
    int socketFd;
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
