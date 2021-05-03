/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_nick.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/03 13:24:19 by jsaariko      ########   odam.nl         */
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
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionNick::execute() {
    Logger::log(LogLevelInfo, "Executing server action NICK");
    try {
        if (params.size() >= requiredParams) {
            if (Utils::String::isAlnum(params[0])) {
                newNickName = &params[0];
                handleNickNameChange();
            } else {
                handleErroneusNickName();
            }
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

void ServerActionNick::handleErroneusNickName() const {
    Connection* connection = server->getConnectionByFd(fd);
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    params.push_back(params[0]);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_ERRONEUSNICKNAME, params));
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

// TODO(Jules): construct general channel replies from functions, maybe under reply?
// TODO(Jules): send ERR_NOLOGIN from action creator, as well as need more params?
