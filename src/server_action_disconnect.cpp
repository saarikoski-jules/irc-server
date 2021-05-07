/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_disconnect.cpp                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:43:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/07 15:18:22 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_disconnect.h"

#include <unistd.h>

#include "server.h"
#include "connection.h"

ServerActionDisconnect::ServerActionDisconnect(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionDisconnect::execute() {
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        // TODO(Jelle) Do thing, coming from SQUIT.
        break;
    case Connection::ClientType:
        disconnectClient();
        break;
    case Connection::NoType:
        disconnectNoType();
        break;
    }
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
