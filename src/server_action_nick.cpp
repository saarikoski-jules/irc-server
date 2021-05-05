/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_nick.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/05 12:10:51 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_nick.h"

#include <vector>

#include "reply.h"
#include "logger.h"
#include "server.h"
#include "utils.h"
#include "connection.h"

#define REQUIRED_SERVER_PARAMS 7

ServerActionNick::ServerActionNick(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionNick::execute() {
    Logger::log(LogLevelInfo, "Executing server action NICK");
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        handleServerNick();
        break;
    case Connection::ClientType:
    case Connection::NoType:
        handleClientNick();
        break;
    }
}

void ServerActionNick::handleServerNick() {
    if (params.size() >= REQUIRED_SERVER_PARAMS) {
        if (server->nicknameExists(params[0]) == false) {
            Connection newConnection;
            Client* client = &newConnection.client;
            newConnection.connectionType = Connection::ClientType;
            client->nickName = params[0];
            client->userName = params[2];
            client->hostName = params[3];
            client->serverName = params[4];  // Currently servertoken here, change?
            client->mode = params[5];  // TODO(Jelle) Validate mode?
            connection->leafConnections.push_back(newConnection);
        } else {
            Logger::log(LogLevelError, "Nickname collision, need to kill colliding users");
            handleNickNameCollision();
        }
    } else {
        Logger::log(LogLevelInfo, "Bad parameter count from server for NICK");
    }
}

void ServerActionNick::handleNickNameCollision() const {
    // TODO(Jelle) Kill nickname when colliding.
}

void ServerActionNick::handleClientNick() {
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
}

void ServerActionNick::handleNickNameChange() const {
    if (server->nicknameExists(*newNickName) == false) {
        Client* client = &connection->client;
        client->nickName = *newNickName;
        // TODO(Jelle) Broadcast nickname/connection to other servers.
        if (client->userName.empty() == false) {
            connection->connectionType = Connection::ClientType;
        }
    } else {
        handleNickNameInUse();
    }
}

void ServerActionNick::handleNickNameInUse() const {
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
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NONICKNAMEGIVEN, params));
}

IServerAction* ServerActionNick::clone() const {
    return (new ServerActionNick(*this));
}

// TODO(Jules): construct general channel replies from functions, maybe under reply?
// TODO(Jules): send ERR_NOLOGIN from action creator, as well as need more params?
