/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 17:46:39 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action.h"

#include <vector>
#include <iostream>

#include "reply.h"
#include "logger.h"
#include "server.h"
#include "utils.h"
#include "connection.h"

ServerActionNick::ServerActionNick(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionNick::execute() {
    Logger::log(LogLevelInfo, "Executing server action NICK");
    try {
        if (params.size() >= requiredParams) {
            newNickName = &params[0];
            handleNickNameChange();
        } else {
            handleNoNicknameGiven();
        }
    } catch (const std::out_of_range& e) {
        Logger::log(LogLevelError, "No valid fd found in NICK action");
        // TODO(Jelle) Handle non valid client fd
    }
}

void ServerActionNick::handleNickNameChange() const {
    if (server->nicknameExists(*newNickName) == false) {
        Connection* connection = server->getConnectionByFd(fd);
        Client* client = &connection->client;
        client->nickName = *newNickName;
        if (client->userName.empty() == false) {
            connection->connectionType = Connection::ClientType;
        }
    } else {
        handleNickNameInUse();
    }
}

void ServerActionNick::handleNickNameInUse() const {
    Connection* connection = server->getConnectionByFd(fd);
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    params.push_back(*newNickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NICKNAMEINUSE, params));
}

void ServerActionNick::handleNoNicknameGiven() const {
    Connection* connection = server->getConnectionByFd(fd);
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NONICKNAMEGIVEN, params));
}

IServerAction* ServerActionNick::clone() const {
    return (new ServerActionNick(*this));
}

ServerActionUser::ServerActionUser(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 4, prefix),
params(params) {}

void ServerActionUser::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    try {
        if (params.size() >= requiredParams) {
            Connection* connection = server->getConnectionByFd(fd);
            Client* client = &connection->client;
            if (connection->connectionType == Connection::NoType) {
                newUserName = &params[0];
                newHostName = &params[1];
                newServerName = &params[2];
                newRealName = &params[3];
                client->userName = *newUserName;
                client->hostName = *newHostName;
                client->serverName = *newServerName;
                client->realName = *newRealName;
                if (client->nickName != "*") {
                    connection->connectionType = Connection::ClientType;
                }
            } else {
                std::vector<std::string> params;
                params.push_back(client->nickName);
                server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_ALREADYREGISTERED, params));
            }
        } else {
            Connection* connection = server->getConnectionByFd(fd);
            std::vector<std::string> params;
            params.push_back(connection->client.nickName);
            params.push_back("USER");
            server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
        }
        // TODO(Jelle) Handle server error's
        // TODO(Jelle) Handle server2server communication
    } catch (const std::out_of_range& e) {
        // TODO(Jelle) Handle non valid client fd
    }
}

IServerAction* ServerActionUser::clone() const {
    return (new ServerActionUser(*this));
}

ServerActionJoin::ServerActionJoin(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionJoin::joinServer(const std::string& name, const std::string& key) {
    Channel* chan = getChannel(name, key);
    std::string reply;
    std::vector<std::string> replyParams;
    Connection* connection = server->getConnectionByFd(fd);
    replyParams.push_back(connection->client.nickName);
    replyParams.push_back(chan->name);
    if (chan->topicIsSet) {
        replyParams.push_back(chan->topic);
        reply = ReplyFactory::newReply(RPL_TOPIC, replyParams);
    } else {
        reply = ReplyFactory::newReply(RPL_NOTOPIC, replyParams);
    }
    server->sendReplyToClient(fd, reply);
}

Channel* ServerActionJoin::getChannel(
    const std::string& name, const std::string& key) {
    Channel* chan;
    try {
        chan = server->findChannel(name);
        chan->addClient(server->getConnectionByFd(fd), key);
    } catch (const std::exception &e) {
        chan = server->createNewChannel(name, fd);
    }
    return (chan);
}

void ServerActionJoin::handleNeedMoreParams() const {
    Connection* connection = server->getConnectionByFd(fd);
    Client* client = &connection->client;
    std::vector<std::string> errParams;
    errParams.push_back(client->nickName);
    errParams.push_back("JOIN");
    std::string errReply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, errParams);
    server->sendReplyToClient(fd, errReply);
}

void ServerActionJoin::execute() {
    Logger::log(LogLevelInfo, "server action join");
    if (params.size() < 1) {
        handleNeedMoreParams();
        return;
    }
    std::vector<std::string> chans = Utils::String::tokenize(params[0], params[0].length(), ",");
    std::vector<std::string> keys;
    if (params.size() > 1) {
        keys = Utils::String::tokenize(params[1], params[1].length(), ",");
    }
    for (size_t i = 0; i < chans.size(); i++) {
        std::string key;
        if (keys.size() > i) {
            key = keys[i];
        } else {
            key = "";
        }
        try {
            joinServer(chans[i], key);
        } catch (const ChannelException& e) {
            server->sendReplyToClient(fd, e.what());
        }
    }
}

IServerAction* ServerActionJoin::clone() const {
    return (new ServerActionJoin(*this));
}

ServerActionAccept::ServerActionAccept(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionAccept::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    server->acceptNewConnection(fd);
}

IServerAction* ServerActionAccept::clone() const {
    return (new ServerActionAccept(*this));
}

ServerActionReceive::ServerActionReceive(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionReceive::execute() {
    Logger::log(LogLevelInfo, "server action receive");
    MessageParser parser;
    std::vector<IServerAction*> newActions = parser.parse(params[0], fd);
    while (!newActions.empty()) {
        server->addNewAction(newActions.front());
        newActions.erase(newActions.begin());
    }
}

IServerAction* ServerActionReceive::clone() const {
    return (new ServerActionReceive(*this));
}

ServerActionDisconnect::ServerActionDisconnect(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionDisconnect::execute() {
    Logger::log(LogLevelInfo, "server action disconnect");
    server->deleteConnection(fd);
}

IServerAction* ServerActionDisconnect::clone() const {
    return (new ServerActionDisconnect(*this));
}
