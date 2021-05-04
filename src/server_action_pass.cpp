/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_pass.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 11:54:07 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/28 16:36:13 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_pass.h"

#include <vector>

#include "connection.h"
#include "utils.h"
#include "server.h"
#include "reply.h"

#define MAXIMUM_TAKEN_PARAMS 4

ServerActionPass::ServerActionPass(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionPass::execute() {
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        break;
    case Connection::ClientType:
        handleAlreadyRegistered();
        break;
    case Connection::NoType:
        if (params.size() >= requiredParams) {
            handlePass();
        } else {
            handleNeedMoreParams();
        }
        break;
    default:
        break;
    }
}

void ServerActionPass::handlePass() const {
    size_t clampedSize = Utils::clamp<size_t>(params.size(), 0, MAXIMUM_TAKEN_PARAMS);
    // No fall through as we use the size as a starting point for the code executed.
    switch (clampedSize) {
    case 4:
        // TODO(Jelle) handle server options better if we choose to do this.
        connection->server.options = params[3];
    case 3: {
        // TODO(Jelle) handle server flag better if we choose to do this.
        std::string fullFlag = params[2];
        size_t splitPoint = fullFlag.find_first_of('|');
        if (splitPoint != std::string::npos
        && fullFlag.find_first_of('|', splitPoint + 1) == std::string::npos) {
            connection->server.implementation = fullFlag.substr(0, splitPoint);
            connection->server.flags = fullFlag.substr(splitPoint + 1, fullFlag.length() - (splitPoint + 1));
        } else {
            // Ignoring invalid flag.
        }
    }
    case 2:
        // TODO(Jelle) handle server irc version better if we choose to do this.
        connection->server.version = params[1];
    case 1:
        connection->password = params[0];
    default:
        break;
    }
}

void ServerActionPass::handleAlreadyRegistered() const {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_ALREADYREGISTERED, params));
}

void ServerActionPass::handleNeedMoreParams() const {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    params.push_back("PASS");
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
}

IServerAction* ServerActionPass::clone() const {
    return (new ServerActionPass(*this));
}
