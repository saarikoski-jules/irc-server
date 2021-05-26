/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_squit.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/26 15:41:10 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/26 15:45:52 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_squit.h"

#include <vector>

#include "server.h"
#include "logger.h"

ServerActionSquit::ServerActionSquit(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionSquit::execute() {
    Logger::log(LogLevelInfo, "Executing server action SQUIT");
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        // TODO(Jelle) Handle clean disconnect of server and message pass through.
        break;
    case Connection::ClientType:
        // Do nothing.
        break;
    case Connection::NoType:
        // Do nothing.
        break;
    }
}

IServerAction* ServerActionSquit::clone() const {
    return (new ServerActionSquit(*this));
}
