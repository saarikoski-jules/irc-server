/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:41:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/15 17:56:15 by jsaariko      ########   odam.nl         */
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
        std::vector<std::string> params, const int& clientFd, Client* client, const std::string& prefix = "");
    void execute();
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
        std::vector<std::string> params, const int& clientFd, Client* client, const std::string& prefix = "");
    void execute();
    ~ServerActionUser() {}
 private:
    std::vector<std::string> params;
};

class ServerActionJoin: public IServerAction {
 public:
    ServerActionJoin(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    void execute();
    ~ServerActionJoin() {}
 private:
    Channel* getChannel(const std::string& name, const std::string& key);
    void joinServer(const std::string& name, const std::string& key);
    void handleNeedMoreParams() const;
    std::vector<std::string> params;
};

class ServerActionMode: public IServerAction {
 public:
    ServerActionMode(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    void execute();
    ~ServerActionMode() {}
 private:
    Channel* getChannel(const std::string& name, const std::string& key);
    void joinServer(const std::string& name, const std::string& key);
    std::vector<std::string> params;
    void modeO(char sign, const std::string& user);
    void editMode(char sign, char mode);
    void setLimit(char sign, const std::string& limit);
    void setKey(char sign, const std::string& key);
    void sendChannelModeIsReply() const;
    void sendUnknownModeReply(char c) const;
    void setBanMask(char sign, const std::string& mask);
    Channel* chan;
};

class ServerActionAccept: public IServerAction {
 public:
    ServerActionAccept(
        std::vector<std::string> params, const int& clientFd, Client* client, const std::string& prefix = "");
    void execute();
    ~ServerActionAccept() {}
 private:
    std::vector<std::string> params;
};

class ServerActionReceive: public IServerAction {
 public:
    ServerActionReceive(
        std::vector<std::string> params, const int& clientFd, Client* client, const std::string& prefix = "");
    void execute();
    ~ServerActionReceive() {}
 private:
    std::vector<std::string> params;
};

class ServerActionDisconnect: public IServerAction {
 public:
    ServerActionDisconnect(
        std::vector<std::string> params, const int& clientFd, Client* client, const std::string& prefix = "");
    void execute();
    ~ServerActionDisconnect() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_H_
