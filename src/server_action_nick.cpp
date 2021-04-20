/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_nick.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/20 12:09:58 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_nick.h"

#include <vector>
#include <iostream>

#include "reply.h"
#include "logger.h"
#include "server.h"
#include "utils.h"
#include "string_conversions.h"

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

// TODO(Jules): construct general channel replies from functions, maybe under reply?
// TODO(Jules): send ERR_NOLOGIN from action creator, as well as need more params?
