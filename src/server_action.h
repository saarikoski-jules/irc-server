/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:41:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/06 18:00:45 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_H_
#define SERVER_ACTION_H_

#include <string>
#include <vector>

#include "iserver_action.h"

class ServerActionNick: public IServerAction {
 public:
    ServerActionNick(std::vector<std::string> params, const int& clientFd);
    void execute(std::vector<Client>& clients);
    ~ServerActionNick() {}
 private:
    std::vector<std::string> params;
};

class ServerActionAccept: public IServerAction {
 public:
    ServerActionAccept(std::vector<std::string> params, const int& clientFd);
    void execute(std::vector<Client>& clients);
    ~ServerActionAccept() {}
 private:
    std::vector<std::string> params;
};

class ServerActionReceive: public IServerAction {
 public:
    ServerActionReceive(std::vector<std::string> params, const int& clientFd);
    void execute(std::vector<Client>& clients);
    ~ServerActionReceive() {}
 private:
    std::vector<std::string> params;
};

class ServerActionDisconnect: public IServerAction {
 public:
    ServerActionDisconnect(std::vector<std::string> params, const int& clientFd);
    void execute(std::vector<Client>& clients);
    ~ServerActionDisconnect() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_H_
