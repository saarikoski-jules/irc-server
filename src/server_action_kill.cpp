/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_kill.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/05 14:22:20 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/05 18:06:20 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_kill.h"

#include <vector>
#include <string>

#include "action_factory.h"
#include "message_parser.h"
#include "server.h"
#include "reply.h"
#include "logger.h"
#include "connection.h"

ServerActionKill::ServerActionKill(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 2, prefix),
params(params) {}

void ServerActionKill::execute() {
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        handleServerKill();
        break;
    case Connection::ClientType:
    case Connection::NoType:
        handleClientKill();
        handleNoPrivileges();
        break;
    }
}

void ServerActionKill::handleServerKill() {
    try {
        if (params.size() >= requiredParams) {
            connection = server->getClientByNick(params[0]);
            if (server->hasLocalConnection(*connection)) {
                handleClientDisconnect();
            } else {
                sendKillMessageToServer();
            }
        } else {
            Logger::log(LogLevelInfo, "Bad parameter count from server for KILL");
        }
    } catch (const std::invalid_argument& e) {
        Logger::log(LogLevelError, "No existing nickname recieved from server to kill");
    }
}

void ServerActionKill::handleClientDisconnect() {
    actionFactory factory;
    std::vector<std::string> disconnectParams;
    disconnectParams.push_back(params[1]);
    server->addNewAction(factory.newAction("DISCONNECT", disconnectParams, connection->fd, prefix));
}

void ServerActionKill::sendKillMessageToServer() {
    std::string killMessage("KILL " + params[0] + " " + params[1]);
    server->sendReplyToClient(fd, killMessage);
}

void ServerActionKill::handleClientKill() {
    if (params.size() >= requiredParams) {
        handleNoPrivileges();
    } else {
        handleNotEnoughParams();
    }
}

void ServerActionKill::handleNoPrivileges() {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NOPRIVILEGES, params));
}

void ServerActionKill::handleNotEnoughParams() {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
}

IServerAction* ServerActionKill::clone() const {
    return (new ServerActionKill(*this));
}
