/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_disconnect.cpp                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:43:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/28 12:25:44 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_disconnect.h"

#include <unistd.h>

#include "server.h"
#include "connection.h"
#include "logger.h"

ServerActionDisconnect::ServerActionDisconnect(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionDisconnect::execute() {
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        disconnectServer();
        break;
    case Connection::ClientType:
        disconnectClient();
        break;
    case Connection::NoType:
        disconnectNoType();
        break;
    }
}

void ServerActionDisconnect::disconnectServer() {
    Logger::log(LogLevelDebug, "Executing DISCONNECT action from server");
    if (params.size() >= 1) {
        disconnectMessage = params[0];
    } else {
        disconnectMessage = "Server split";
    }
    server->sendErrorToConnectionBypassingQueue(fd, disconnectMessage);
    close(fd);
    handleSplitClients();
    handleSplitServer();
    server->deleteConnection(fd);
}

void ServerActionDisconnect::handleSplitClients() {
    for (std::vector<Connection*>::iterator i = connection->leafConnections.begin(); i != connection->leafConnections.end(); i++) {
        const Connection* leafConnection = *i;
        if (leafConnection->connectionType == Connection::ClientType) {
            std::string reply(":" + leafConnection->client.nickName + " QUIT :" + disconnectMessage + "\r\n");
            server->sendMessageToAllServersButOne(reply, fd);
            server->sendMessageToAllLocalUsersInClientChannels(leafConnection, reply);
            server->removeClientFromChannels(leafConnection);
        }
    }
}

void ServerActionDisconnect::handleSplitServer() {
    for (std::vector<Connection*>::iterator i = connection->leafConnections.begin(); i != connection->leafConnections.end(); i++) {
        const Connection* leafConnection = *i;
        if (leafConnection->connectionType == Connection::ServerType) {
            std::string reply(":" + Server::serverName + " SQUIT " + leafConnection->server.name + " :" + disconnectMessage + "\r\n");
            server->sendMessageToAllServersButOne(reply, fd);
        }
    }
    std::string reply(":" + Server::serverName + " SQUIT " + connection->server.name + " :" + disconnectMessage + "\r\n");
    server->sendMessageToAllServersButOne(reply, fd);
}

void ServerActionDisconnect::disconnectClient() {
    if (params.size() >= 1) {
        disconnectMessage = params[0];
    } else {
        disconnectMessage = "EOF from client";
    }
    server->sendErrorToConnectionBypassingQueue(fd, disconnectMessage);
    close(fd);
    std::string reply(":" + connection->client.nickName + " QUIT :" + disconnectMessage + "\r\n");
    server->sendMessageToAllServers(reply);
    server->sendMessageToAllLocalUsersInClientChannels(connection, reply);
    server->removeClientFromChannels(connection);
    server->deleteConnection(fd);
}

void ServerActionDisconnect::disconnectNoType() {
    if (params.size() >= 1) {
        disconnectMessage = params[0];
    } else {
        disconnectMessage = "EOF from client";
    }
    server->sendErrorToConnectionBypassingQueue(fd, disconnectMessage);
    close(fd);
    server->deleteConnection(fd);
}

IServerAction* ServerActionDisconnect::clone() const {
    return (new ServerActionDisconnect(*this));
}
