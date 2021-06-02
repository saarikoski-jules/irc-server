/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_who.cpp                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/06/01 11:58:58 by jules        #+#    #+#                  */
/*   Updated: 2021/06/01 13:23:46 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_who.h"

#include "connection.h"
#include "server.h"
#include "construct_reply.h"

ServerActionWho::ServerActionWho(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {
	Connection* connection = server->getConnectionByFd(fd);
	if (params.size() < requiredParams) {
		std::vector<std::string> params;
		switch (connection->connectionType) {
			case Connection::ServerType:
				try {
					connection = connection->getLeafConnection(prefix);

				} catch (const std::exception& e) {
					throw std::invalid_argument("Invalid prefix");
				}
			case Connection::ClientType:
				break;
			case Connection::NoType:
				server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
				throw std::invalid_argument("Bad user");
				break;
		}
		params.push_back(connection->client.nickName);
		params.push_back("WHO");
		server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
		throw std::invalid_argument("Not enough params");
	}
	if (connection->connectionType == Connection::NoType) {
		server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
		throw std::invalid_argument("Client not registered");
	}
}

void ServerActionWho::execute() {
	Connection* connection = server->getConnectionByFd(fd);
	if (connection->connectionType == Connection::ClientType) {
		try {
			Channel* chan = server->findChannel(params[0]);
			std::vector<Connection*> users = chan->getConnections();
			for (std::vector<Connection*>::iterator i = users.begin(); i != users.end(); i++) {
				std::vector<std::string> whoReplyParams;
				whoReplyParams.push_back(connection->client.nickName);
				whoReplyParams.push_back(chan->name);
				whoReplyParams.push_back((*i)->client.userName);
				whoReplyParams.push_back((*i)->client.hostName);
				whoReplyParams.push_back((*i)->client.serverName);
				whoReplyParams.push_back((*i)->client.nickName);
				if (chan->isOper(*i)) {
					whoReplyParams.push_back(std::string("H@"));
				} else {
					whoReplyParams.push_back(std::string("H+"));
				}
				whoReplyParams.push_back("0");
				whoReplyParams.push_back((*i)->client.realName);
				std::string reply = ReplyFactory::newReply(RPL_WHOREPLY, whoReplyParams);
				server->sendReplyToClient(fd, reply);
			}
			std::vector<std::string> eoWhoReplyParams;

			eoWhoReplyParams.push_back(connection->client.nickName);
			eoWhoReplyParams.push_back(chan->name);
			std::string endReply = ReplyFactory::newReply(RPL_ENDOFWHO, eoWhoReplyParams);
			server->sendReplyToClient(fd, endReply);
		} catch (const std::exception& e) {
			std::vector<std::string> params;
			params.push_back(connection->client.nickName);
			params.push_back(params[0]);
			server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NOSUCHSERVER, params));
		}
	}
}

IServerAction* ServerActionWho::clone() const {
	return (new ServerActionWho(*this));
}
