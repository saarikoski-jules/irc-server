/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_topic.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/07 15:24:48 by jules         #+#    #+#                 */
/*   Updated: 2021/05/10 16:26:01 by jsaariko      ########   odam.nl         */
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
				// throw std::invalid_argument("Bad user");//TODO(Jules): yeah how about you don't
				break;
		}
		params.push_back(connection->client.nickName);
		server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
		// throw std::invalid_argument("Not enough params");
	}
}

void ServerActionTopic::execute() {
	connection = server->getConnectionByFd(fd);
	if (connection->connectionType == Connection::ServerType) {
		connection = connection->getLeafConnection(prefix);
	}
	try {
		chan = server->findChannel(params[0]);
	} catch (const channelException& e) {
		//TODO(Jules): send error no such channel
	}
	if (params.size() > 1) {
		changeTopic();
	} else {
		checkTopic();
	}
}
#include <iostream>
void ServerActionTopic::changeTopic() {
	// replyParams.push_back(chan->topic);
	//TODO(Jules): Change channel topic if client has rights
	std::vector<std::string> replyParams;
	replyParams.push_back(connection->client.nickName);
	replyParams.push_back(chan->name);
	const std::string modes = chan->getModes();
	const Connection const_con = *connection;
	if (modes.find('t') != std::string::npos && !chan->isOper(connection)) {
		server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_CHANOPRIVSNEEDED, replyParams));
	} else {
		std::cout << "param 0 " << params[0] << std::endl;
		std::cout << "param 1 " << params[1] << std::endl;
		chan->topic = params[1];
		std::vector<Connection*> broadcastTo = chan->getConnections(const_con);
		std::string sentFrom(connection->client.nickName + "!" + connection->client.userName + "@" + connection->client.hostName);
		replyParams.push_back(chan->topic);
		for (std::vector<Connection*>::iterator it = broadcastTo.begin(); it != broadcastTo.end(); it++) {
			server->sendReplyToClient((*it)->fd, std::string("TOPIC " + chan->name + " :" + chan->topic), sentFrom);
		}
		server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_TOPIC, replyParams));
	}
}

void ServerActionTopic::checkTopic() const {
	std::vector<std::string> replyParams;
	replyParams.push_back(connection->client.nickName);
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
