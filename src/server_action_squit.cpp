/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_squit.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/26 15:41:10 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/28 13:12:37 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_squit.h"

#include <vector>
#include <stdexcept>

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
        handleServerSquit();
        break;
    case Connection::ClientType:
        // Do nothing.
        break;
    case Connection::NoType:
        // Do nothing.
        break;
    }
}

void ServerActionSquit::handleServerSquit() {
    try {
        if (params.size() >= requiredParams) {
            Logger::log(LogLevelInfo, "Attempting to delete server from SQUIT");
            connection->removeLeafServerByName(params[0]);
            if (params.size() >= 2) {
                quitMessage = params[1];
            } else {
                quitMessage = "Server split";
            }
            std::string reply(":" + Server::serverName + " SQUIT " + params[0] + " :" + quitMessage + "\r\n");
            server->sendMessageToAllServersButOne(reply, fd);
        } else {
            Logger::log(LogLevelError, "Not enough paramaters in server SQUIT");
        }
    } catch (const std::out_of_range& e) {
        // No server found from argument.
    }
}

IServerAction* ServerActionSquit::clone() const {
    return (new ServerActionSquit(*this));
}
