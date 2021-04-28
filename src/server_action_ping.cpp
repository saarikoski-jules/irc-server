/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_ping.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 17:03:35 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/28 18:00:56 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_ping.h"

#include <string>
#include <vector>

#include "server.h"
#include "reply.h"

ServerActionPing::ServerActionPing(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionPing::execute() {
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ClientType:
        handlePingClient();
        break;
    case Connection::ServerType:
        handlePingServer();
        break;
    case Connection::NoType:
        handleNotRegistered();
        break;
    default:
        break;
    }
}

void ServerActionPing::handlePingClient() const {
    if (params.size() >= requiredParams) {
        if (params.size() == 1 || params[1] == SERVERNAME) {
            std::string reply("PONG " SERVERNAME " :" + params[0]);
            server->sendReplyToClient(fd, reply);
        } else {
            // FORMAT :sender to-send-to
            // TODO(Jelle) Find server and send through or error handle.
            // FORMAT :sendernick PING sendernick :to-send-to
        }
    } else {
        handleNotEnoughParams();
    }
}

void ServerActionPing::handleNotEnoughParams() const {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    params.push_back("PING");
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
}

void ServerActionPing::handlePingServer() const {
    if (params.size() >= requiredParams) {
        if (params.size() == 1 || params[1] == SERVERNAME) {
            std::string reply("PONG " SERVERNAME " :" + params[0]);
            server->sendReplyToClient(fd, reply);
        } else {
            // FORMAT :sender to-send-to
            // TODO(Jelle) Find server and send through or error handle.
            // FORMAT :sender PING sender to-send-to
        }
    } else {
        // TODO(Jelle) Handle Error and kill server.
    }
}

void ServerActionPing::handleNotRegistered() const {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NOTREGISTERED, params));
}

IServerAction* ServerActionPing::clone() const {
    return (new ServerActionPing(*this));
}
