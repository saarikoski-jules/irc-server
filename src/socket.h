/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:48:54 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/02 23:20:24 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <string>
#include <queue>

#include "server_action.h"

#define MAX_MESSAGE_SIZE 512

class Socket {
 public:
    explicit Socket(std::queue<ServerAction>* actions);
    void bindAndListenToPort(const int& port);
    void openConnection();
    void receiveData(const int& clientFd);
    void sendData(const int& clientFd, const std::string& msg) const;
 private:
    Socket();
    int socketFd;
    struct sockaddr_in addr;
    std::queue<ServerAction>* actions;
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
