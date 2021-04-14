/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/14 16:29:40 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action.h"

#include <vector>
#include <iostream>

#include "reply.h"
#include "logger.h"
#include "server.h"

ServerActionNick::ServerActionNick(
    std::vector<std::string> params, const int& clientFd, const Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

void ServerActionNick::execute() {
    Logger::log(LogLevelInfo, "Executing server action NICK");
    try {
        client = server->getClientByFd(clientFd);
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
        client->nickName = *newNickName;
        if (client->userName.empty() == false) {
            client->registered = true;
        }
    } else {
        handleNickNameInUse();
    }
}

void ServerActionNick::handleNickNameInUse() const {
    std::vector<std::string> params;
    params.push_back(client->nickName);
    params.push_back(*newNickName);
    server->sendReplyToClient(clientFd, ReplyFactory::newReply(ERR_NICKNAMEINUSE, params));
}

void ServerActionNick::handleNoNicknameGiven() const {
    std::vector<std::string> params;
    params.push_back(client->nickName);
    server->sendReplyToClient(clientFd, ReplyFactory::newReply(ERR_NONICKNAMEGIVEN, params));
}

ServerActionUser::ServerActionUser(
    std::vector<std::string> params, const int& clientFd, const Client* cli, const std::string& prefix) :
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
            Client* client = server->getClientByFd(clientFd);
            client->userName = newUserName;
            client->hostName = newHostName;
            client->serverName = newServerName;
            client->realName = newRealName;
            if (client->nickName != "*") {
                client->registered = true;
            }
            std::cout << *client << std::endl;
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

ServerActionAccept::ServerActionAccept(
    std::vector<std::string> params, const int& clientFd, const Client* cli, const std::string& prefix) :
IServerAction(clientFd, 0, cli, prefix),
params(params) {}

void ServerActionAccept::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    server->acceptNewClient(clientFd);
}

ServerActionReceive::ServerActionReceive(
    std::vector<std::string> params, const int& clientFd, const Client* cli, const std::string& prefix) :
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
    std::vector<std::string> params, const int& clientFd, const Client* cli, const std::string& prefix) :
IServerAction(clientFd, 0, cli, prefix),
params(params) {}

void ServerActionDisconnect::execute() {
    Logger::log(LogLevelInfo, "server action disconnect");
    server->deleteClient(clientFd);
}
