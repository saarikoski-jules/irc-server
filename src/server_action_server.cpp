/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_server.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 15:02:33 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/28 16:45:12 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_server.h"

#include <string>
#include <vector>

#include "logger.h"
#include "server.h"

ServerActionServer::ServerActionServer(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 4, prefix),
params(params) {}

void ServerActionServer::execute() {
    Logger::log(LogLevelDebug, "Executing SERVER action.");
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        // TODO(Jelle) Register new server to list.
        break;
    case Connection::ClientType:
        handleAlreadyRegistered();
        break;
    case Connection::NoType:
        handleServer();
    default:
        break;
    }
}

void ServerActionServer::handleServer() const {
    if (params.size() >= requiredParams) {
        if (connection->password == SERVER_CONNECTION_PASSWORD) {
            ServerConnection* serverConnection = &connection->server;
            serverConnection->name = params[0];
            serverConnection->hopcount = params[1];  // TODO(Jelle) If needed, convert to uint16.
            if (server->serverTokenExists(params[2]) == false) {
                serverConnection->token = params[2];
            } else {
                // TODO(Jelle) Send error reply.
            }
            serverConnection->info = params[3];
            connection->connectionType = Connection::ServerType;
        } else {
            // TODO(Jelle) handle no password error.
        }
    } else {
        handleNeedMoreParams();
    }
}

void ServerActionServer::handleAlreadyRegistered() const {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_ALREADYREGISTERED, params));
}

void ServerActionServer::handleNeedMoreParams() const {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    params.push_back("SERVER");
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
}

IServerAction* ServerActionServer::clone() const {
    return (new ServerActionServer(*this));
}
