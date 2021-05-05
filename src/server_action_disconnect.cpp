/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_disconnect.cpp                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:43:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/05 17:51:00 by jvisser       ########   odam.nl         */
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
        // TODO(Jelle) Do thing.
        break;
    case Connection::ClientType:
        disconnectClient();
        break;
    case Connection::NoType:
        // TODO(Jelle) Do thing.
        break;
    }
}

void ServerActionDisconnect::disconnectClient() const {
    if (params.size() >= 1) {
        server->sendErrorToConnectionBypassingQueue(fd, params[0]);
    } else {
        server->sendErrorToConnectionBypassingQueue(fd, "Disconnecting connection");
    }
    close(fd);
    // TODO(Jelle) QUIT message of the client to all servers.
    server->deleteConnection(fd);
}

IServerAction* ServerActionDisconnect::clone() const {
    return (new ServerActionDisconnect(*this));
}
