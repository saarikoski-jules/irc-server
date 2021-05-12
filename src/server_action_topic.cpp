/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_topic.cpp                           :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/07 15:24:48 by jules         #+#    #+#                 */
/*   Updated: 2021/05/12 16:32:21 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_topic.h"
#include "construct_reply.h"

ServerActionTopic::ServerActionTopic(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {
	if (params.size() < requiredParams) {
		Connection* connection = server->getConnectionByFd(fd);
		std::vector<std::string> params;
		switch(connection->connectionType) {
			case Connection::ServerType:
				connection = connection->getLeafConnection(prefix);
			case Connection::ClientType:
				break;
			case Connection::NoType:
				server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
				throw std::invalid_argument("Bad user");//TODO(Jules): yeah how about you don't
				break;
		}
		params.push_back(connection->client.nickName);
		params.push_back("TOPIC");
		server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
		throw std::invalid_argument("Not enough params");
	}
}

void ServerActionTopic::execute() {
	connection = server->getConnectionByFd(fd);
	std::string clientNick;
	if (connection->connectionType == Connection::ServerType) {
		Connection* tmp = connection->getLeafConnection(prefix);
		clientNick = tmp->client.nickName;
	} else {
		clientNick = connection->client.nickName;
	}
	try {
		chan = server->findChannel(params[0]);
	} catch (const ChannelException& e) {
		//TODO(Jules): send error no such channel
	}
	if (params.size() > 1) {
		changeTopic(clientNick);
	} else {
		checkTopic(clientNick);
	}
}

void ServerActionTopic::changeTopic(const std::string& clientNick) {
	std::vector<std::string> replyParams;
	replyParams.push_back(clientNick);
	replyParams.push_back(chan->name);
	const std::string modes = chan->getModes();
	const Connection const_con = *connection;
	if (modes.find('t') != std::string::npos && !chan->isOper(connection)) {
		server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_CHANOPRIVSNEEDED, replyParams));
	} else {
		chan->topic = params[1];
		chan->topicIsSet = true;
		std::vector<Connection*> broadcastTo = chan->getConnections(const_con);
		
		std::string sentFrom;
		if (connection->connectionType == Connection::ClientType) {
			sentFrom = std::string(connection->client.nickName + "!" + connection->client.userName + "@" + connection->client.hostName);
		} else if (connection->connectionType == Connection::ServerType) {
			sentFrom = prefix;
		}
		replyParams.push_back(chan->topic);
		for (std::vector<Connection*>::iterator it = broadcastTo.begin(); it != broadcastTo.end(); it++) {
			if (server->hasLocalConnection(**it)) {
				server->sendReplyToClient((*it)->fd, std::string("TOPIC " + chan->name + " :" + chan->topic), sentFrom);
			}
		}
		server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_TOPIC, replyParams));
		if (chan->name[0] == '#') {
			if (connection->connectionType == Connection::ClientType) {
				server->sendMessageToAllServers(std::string(sentFrom + "TOPIC " + chan->name + " :" + chan->topic));
			} else if (connection->connectionType == Connection::ServerType) {
				server->sendMessageToAllServersButOne(std::string(sentFrom + "TOPIC " + chan->name + " :" + chan->topic), fd);
			}
		}
	}
}

void ServerActionTopic::checkTopic(const std::string& clientNick) const {
	std::vector<std::string> replyParams;
	replyParams.push_back(clientNick);
	replyParams.push_back(chan->name);

	if (chan->topic == "") {
		server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_NOTOPIC, replyParams));
	} else {
		replyParams.push_back(chan->topic);
		server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_TOPIC, replyParams));
	}
}


IServerAction* ServerActionTopic::clone() const {
	return (new ServerActionTopic(*this));
}
