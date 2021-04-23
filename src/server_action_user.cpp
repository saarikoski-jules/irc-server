/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_user.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:34:39 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:23:46 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_user.h"

#include <iostream>
#include <vector>
#include <string>

#include "server.h"
#include "logger.h"

ServerActionUser::ServerActionUser(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 4, prefix),
params(params) {}

void ServerActionUser::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    try {
        if (params.size() >= requiredParams) {
            Connection* connection = server->getConnectionByFd(fd);
            Client* client = &connection->client;
            if (connection->connectionType == Connection::NoType) {
                newUserName = &params[0];
                newHostName = &params[1];
                newServerName = &params[2];
                newRealName = &params[3];
                client->userName = *newUserName;
                client->hostName = *newHostName;
                client->serverName = *newServerName;
                client->realName = *newRealName;
                if (client->nickName != "*") {
                    connection->connectionType = Connection::ClientType;
                }
            } else {
                std::vector<std::string> params;
                params.push_back(client->nickName);
                server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_ALREADYREGISTERED, params));
            }
        } else {
            Connection* connection = server->getConnectionByFd(fd);
            std::vector<std::string> params;
            params.push_back(connection->client.nickName);
            params.push_back("USER");
            server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
        }
        // TODO(Jelle) Handle server error's
        // TODO(Jelle) Handle server2server communication
    } catch (const std::out_of_range& e) {
        // TODO(Jelle) Handle non valid client fd
    }
}

