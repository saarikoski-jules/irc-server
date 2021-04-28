/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:48:54 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/23 17:52:03 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netinet/in.h>
#include <string>
#include <queue>
#include <vector>
#include <map>

#include "iserver_action.h"
#include "server_connection.h"
#include "client.h"
#include "connection.h"

#define MAX_MESSAGE_SIZE 512

class Socket {
 public:
    explicit Socket(std::queue<IServerAction*>* actions);
    void bindAndListenToPort(const int& port);
    void checkNewConnections();
    void checkConnectionAndNewData(std::map<const int, Connection>* connections);
    void sendData(const int& clientFd, const std::string& msg);
 private:
    Socket();
    int socketFd;
    struct sockaddr_in addr;
    fd_set readSet;
    fd_set writeSet;
    int createFdSet(std::map<const int, Connection>* connections);
    void readFromFds(const int& maxFd);
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
    std::string fullMessage;
};

#endif  //  SOCKET_H_
