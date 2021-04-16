/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/16 09:47:10 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action.h"

#include <vector>
#include <iostream>

#include "reply.h"
#include "logger.h"
#include "server.h"
#include "utils.h"

ServerActionNick::ServerActionNick(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

void ServerActionNick::execute() {
    Logger::log(LogLevelInfo, "Executing server action NICK");
    try {
        if (params.size() >= 1) {
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
        cli->nickName = *newNickName;
        if (cli->userName.empty() == false) {
            cli->registered = true;
        }
    } else {
        handleNickNameInUse();
    }
}

void ServerActionNick::handleNickNameInUse() const {
    std::vector<std::string> params;
    params.push_back(cli->nickName);
    params.push_back(*newNickName);
    server->sendReplyToClient(clientFd, ReplyFactory::newReply(ERR_NICKNAMEINUSE, params));
}

void ServerActionNick::handleNoNicknameGiven() const {
    std::vector<std::string> params;
    params.push_back(cli->nickName);
    server->sendReplyToClient(clientFd, ReplyFactory::newReply(ERR_NONICKNAMEGIVEN, params));
}

ServerActionUser::ServerActionUser(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 4, cli, prefix),
params(params) {}

void ServerActionUser::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    try {
        const std::string& newUserName = params[0];
        const std::string& newHostName = params[1];
        const std::string& newServerName = params[2];
        const std::string& newRealName = params[3];
        if (server->usernameExists(newUserName) == false) {
            cli->userName = newUserName;
            cli->hostName = newHostName;
            cli->serverName = newServerName;
            cli->realName = newRealName;
            if (cli->nickName != "*") {
                cli->registered = true;
            }
            std::cout << *cli << std::endl;
        } else {
            Logger::log(LogLevelError, "Something went wrong while setting user info");
            // server->sendErrorToClient(ERR_NONICKNAMEGIVEN, clientFd);
        }
        // TODO(Jelle) Handle server error's
        // TODO(Jelle) Handle server2server communication
    } catch (const std::out_of_range& e) {
        // TODO(Jelle) Handle non valid client fd
    }
}

ServerActionJoin::ServerActionJoin(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

Channel* ServerActionJoin::getChannel(
    const std::string& name, const std::string& key) {
    Channel* chan;
    try {
        chan = server->findChannel(name);
        chan->addClient(server->getClientByFd(clientFd), key);
    } catch (const std::exception &e) {
        chan = server->createNewChannel(name, clientFd);
    }
    return (chan);
}

void ServerActionJoin::joinServer(const std::string& name, const std::string& key) {
    Channel* chan = getChannel(name, key);
    std::string reply;
    std::vector<std::string> replyParams;
    replyParams.push_back(cli->nickName);
    replyParams.push_back(chan->name);
    if (chan->topicIsSet) {
        replyParams.push_back(chan->topic);
        reply = ReplyFactory::newReply(RPL_TOPIC, replyParams);
    } else {
        reply = ReplyFactory::newReply(RPL_NOTOPIC, replyParams);
    }
    server->sendReplyToClient(clientFd, reply);
}

void ServerActionJoin::handleNeedMoreParams() const {
    std::vector<std::string> errParams;
    errParams.push_back(cli->nickName);
    errParams.push_back("JOIN");
    std::string errReply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, errParams);
    server->sendReplyToClient(clientFd, errReply);
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
            server->sendReplyToClient(clientFd, e.what());
        }
    }
}

ServerActionAccept::ServerActionAccept(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 0, cli, prefix),
params(params) {}

void ServerActionAccept::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    server->acceptNewClient(clientFd);
}

ServerActionReceive::ServerActionReceive(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

void ServerActionReceive::execute() {
    Logger::log(LogLevelInfo, "server action receive");
    MessageParser parser;
    std::vector<IServerAction*> newActions = parser.parse(params[0], clientFd, cli);
    while (!newActions.empty()) {
        server->addNewAction(newActions.front());
        newActions.erase(newActions.begin());
    }
}

ServerActionDisconnect::ServerActionDisconnect(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 0, cli, prefix),
params(params) {}

void ServerActionDisconnect::execute() {
    Logger::log(LogLevelInfo, "server action disconnect");
    server->deleteClient(clientFd);
}
