/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:41:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/09 18:21:31 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_H_
#define SERVER_ACTION_H_

#include <string>
#include <vector>

#include "iserver_action.h"
// #include "server.h"
class Server;

class ServerActionNick: public IServerAction {
 public:
    ServerActionNick(std::vector<std::string> params, const int& clientFd, const std::string& prefix);
    void execute(Server*);
    ~ServerActionNick() {}
 private:
    std::vector<std::string> params;
};

class ServerActionUser: public IServerAction {
 public:
    ServerActionUser(std::vector<std::string> params, const int& clientFd, const std::string& prefix);
    void execute(Server*);
    ~ServerActionUser() {}
 private:
    std::vector<std::string> params;
};

class ServerActionAccept: public IServerAction {
 public:
    ServerActionAccept(std::vector<std::string> params, const int& clientFd);
    void execute(Server*);
    ~ServerActionAccept() {}
 private:
    std::vector<std::string> params;
};

class ServerActionReceive: public IServerAction {
 public:
    ServerActionReceive(std::vector<std::string> params, const int& clientFd);
    void execute(Server*);
    ~ServerActionReceive() {}
 private:
    std::vector<std::string> params;
};

class ServerActionDisconnect: public IServerAction {
 public:
    ServerActionDisconnect(std::vector<std::string> params, const int& clientFd);
    void execute(Server*);
    ~ServerActionDisconnect() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_H_
