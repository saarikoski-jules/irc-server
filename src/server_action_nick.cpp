/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_nick.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/12 15:37:09 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_nick.h"

#include <vector>

#include "reply.h"
#include "logger.h"
#include "server.h"
#include "utils.h"
#include "connection.h"
#include "action_factory.h"
#include "construct_reply.h"

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
    if (params.size() >= requiredParams) {
        if (params.size() >= REQUIRED_SERVER_PARAMS) {
            if (server->nicknameExists(params[0]) == false) {
                Connection newConnection;
                Client* client = &newConnection.client;
                newConnection.fd = fd;
                newConnection.connectionType = Connection::ClientType;
                client->nickName = params[0];
                client->userName = params[2];
                client->hostName = params[3];
                client->serverName = params[4];  // Currently servertoken here, change?
                client->mode = params[5];  // TODO(Jelle) Validate mode?
                client->realName = params[6];
                connection->leafConnections.push_back(newConnection);
                std::string reply = constructServerNickBroadcast(params);
                server->sendMessageToAllServersButOne(reply, fd);
            } else {
                Logger::log(LogLevelError, "Nickname collision, need to kill colliding users");
                handleNickNameCollision();
            }
        } else {
            try {
                Connection* leafConnection = connection->getLeafConnection(prefix);
                newNickName = &params[0];
                if (server->nicknameExists(*newNickName) == false) {
                    std::string oldNickName = leafConnection->client.nickName;
                    leafConnection->client.nickName = *newNickName;
                    std::string reply = constructNickChangeBroadcast(oldNickName, *newNickName);
                    server->sendMessageToAllServersButOne(reply, fd);
                } else {
                    handleNickNameCollision();
                    params[0] = prefix;
                    handleNickNameCollision();
                }
            } catch (const std::out_of_range& e) {
                Logger::log(LogLevelError, "Unknown nickname from server for NICK");
                Logger::log(LogLevelError, e.what());
            }
        }
    } else {
        Logger::log(LogLevelInfo, "Bad parameter count from server for NICK");
    }
}

void ServerActionNick::handleNickNameCollision() const {
    sendKillMessageToServer();
    scheduleConnectionCollisionKill();
}

void ServerActionNick::sendKillMessageToServer() const {
    std::string killMessage("KILL " + params[0] + " :Nickname collision");
    server->sendReplyToClient(fd, killMessage);
}

void ServerActionNick::scheduleConnectionCollisionKill() const {
    actionFactory factory;
    std::vector<std::string> killParams;
    killParams.push_back(params[0]);  // Nickname to kill.
    killParams.push_back("Nickname collision");  // Message to give to user and servers.
    server->addNewAction(factory.newAction("KILL", killParams, fd));
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
        std::string oldNickName = client->nickName;
        client->nickName = *newNickName;
        if (client->userName.empty() == false) {
            if (connection->connectionType == Connection::ClientType) {
                std::string reply = constructNickChangeBroadcast(oldNickName, *newNickName);
                server->sendMessageToAllServers(reply);
            } else {
                connection->connectionType = Connection::ClientType;
                std::string reply = constructNewNickBroadcast(*connection);
                server->sendMessageToAllServers(reply);
            }
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
