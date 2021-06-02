/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_server.cpp                          :+:    :+:             */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 15:02:33 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/02 11:10:22 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_server.h"

#include <string>
#include <vector>

#include "logger.h"
#include "server.h"
#include "construct_reply.h"

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
        handleServerFromServer();
        break;
    case Connection::ClientType:
        handleAlreadyRegistered();
        break;
    case Connection::NoType:
        handleServerRegistration();
        break;
    default:
        break;
    }
}

void ServerActionServer::handleServerFromServer() const {
    Logger::log(LogLevelDebug, "Attempting SERVER registration.");
    if (params.size() >= requiredParams) {
        // TODO(Jelle) Is server collision a thing?
        // if (server->serverTokenExists(params[2]) == false) {
            Connection* newConnection = new Connection();
            ServerConnection* serverConnection = &newConnection->server;
            newConnection->fd = fd;
            newConnection->connectionType = Connection::ServerType;
            serverConnection->name = params[0];
            serverConnection->hopcount = params[1];
            serverConnection->token = params[2];
            serverConnection->info = params[3];
            std::string reply = constructNewServerBroadcast(*newConnection);
            server->sendMessageToAllServersButOne(reply, fd);
            connection->leafConnections.push_back(newConnection);
        // } else {
        //     server->sendErrorToConnectionBypassingQueue(fd, "Server token already exists, not registering");
        // }
    }
}

void ServerActionServer::handleServerRegistration() const {
    Logger::log(LogLevelDebug, "Attempting new SERVER registration.");
    if (params.size() >= requiredParams) {
        if (connection->password == SERVER_CONNECTION_PASSWORD) {
            // if (server->serverTokenExists(params[2]) == false) {
                ServerConnection* serverConnection = &connection->server;
                serverConnection->name = params[0];
                serverConnection->hopcount = params[1];
                serverConnection->token = params[2];
                serverConnection->info = params[3];
                connection->connectionType = Connection::ServerType;
                std::string reply;
                if (connection->isStartingServer == false) {
                    reply = constructServerValidation();
                    server->sendMessageToServer(fd, reply);
                }
                reply = constructNewServerBroadcast(*connection);
                server->sendMessageToAllServersButOne(reply, fd);
                server->burstServerInformationTo(fd);
            // } else {
            //     server->sendErrorToConnectionBypassingQueue(fd, "Server token already exists, not registering");
            // }
        } else {
            server->sendErrorToConnectionBypassingQueue(fd, "Unable to authorize");
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
