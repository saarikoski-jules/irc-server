/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:48:54 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 12:07:27 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <string>
#include <queue>
#include <vector>

#include "iserver_action.h"
#include "client.h"

#define MAX_MESSAGE_SIZE 512

class Socket {
 public:
    explicit Socket(std::queue<IServerAction*>* actions);
    void bindAndListenToPort(const int& port);
    void checkNewConnections();
    void checkConnectionAndNewDataFrom(std::vector<Client>* clients);
    void sendData(const int& clientFd, const std::string& msg) const;
 private:
    Socket();
    int socketFd;
    struct sockaddr_in addr;
    int createFdSet(std::vector<Client>* clients, fd_set* set);
    void readFromFds(std::vector<Client>* clients, fd_set readSet);
    std::queue<IServerAction*>* actions;
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
