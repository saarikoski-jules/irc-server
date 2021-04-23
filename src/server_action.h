/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:41:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 17:27:57 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_H_
#define SERVER_ACTION_H_

#include <string>
#include <vector>

#include "iserver_action.h"
#include "client.h"
#include "channel.h"

class Server;

class ServerActionNick: public IServerAction {
 public:
    ServerActionNick(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionNick() {}
 private:
    void handleNickNameChange() const;
    void handleNickNameInUse() const;
    void handleNoNicknameGiven() const;
    std::vector<std::string> params;
    const std::string* newNickName;
};

class ServerActionUser: public IServerAction {
 public:
    ServerActionUser(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionUser() {}
 private:
    std::vector<std::string> params;
    const std::string* newUserName;
    const std::string* newHostName;
    const std::string* newServerName;
    const std::string* newRealName;
};

class ServerActionJoin: public IServerAction {
 public:
    ServerActionJoin(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionJoin() {}
 private:
    Channel* getChannel(const std::string& name, const std::string& key);
    void joinServer(const std::string& name, const std::string& key);
    void handleNeedMoreParams() const;
    std::vector<std::string> params;
};

class ServerActionAccept: public IServerAction {
 public:
    ServerActionAccept(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionAccept() {}
 private:
    std::vector<std::string> params;
};

class ServerActionReceive: public IServerAction {
 public:
    ServerActionReceive(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionReceive() {}
 private:
    std::vector<std::string> params;
};

class ServerActionDisconnect: public IServerAction {
 public:
    ServerActionDisconnect(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionDisconnect() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_H_
