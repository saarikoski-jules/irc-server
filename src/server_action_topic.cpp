/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_topic.cpp                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/07 15:24:48 by jules        #+#    #+#                  */
/*   Updated: 2021/05/07 16:13:30 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_topic.h"

ServerActionTopic::ServerActionTopic(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {
	if (params.size() < requiredParams) {
		Connection* connection = server->getConnectionByFd(fd);
		std::vector<std::string> params;
		switch(connection.connectionType) {
			case Connection::ServerType:
				connection = connection.getLeafConnection(prefix);
			case Connection::ClientType:
				break;
			case Connection::NoType:
				server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nick));
		}
		params.push_back(connection->client.nick);
		server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
		throw invalid_argument("Not enough params");
	}
}

void ServerActionTopic::execute() {
	Connection* connection = server->getConnectionByFd(fd);
	if (connection == Connection::ServerType) {
		connection = connection.getLeafConnection(prefix);
	}
	std::vector<std::string> params;
	params.push_back(connection->client.nick);
	Channel* chan = server->findChannel(params[0]);
	if (params.size() > 1) {
		params.push_back(chan->topic);
		//TODO(Jules): Change channel topic if client has rights
		server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_TOPIC, params));
	} else {
		if (chan->topic == "") {
			server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_NOTOPIC, params));
		} else {
			params.push_back(chan->topic);
			server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_TOPIC, params));
		}
	}
}

IServerAction* ServerActionTopic::clone() const {
	return (new ServerActionTopic(*this));
}
