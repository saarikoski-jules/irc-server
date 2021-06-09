/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_topic.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/07 15:24:48 by jules         #+#    #+#                 */
/*   Updated: 2021/06/09 16:45:10 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_topic.h"

#include "construct_reply.h"
#include "logger.h"

ServerActionTopic::ServerActionTopic(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {
	Connection* connection = server->getConnectionByFd(fd);
	if (params.size() < requiredParams) {
		std::vector<std::string> params;
		switch(connection->connectionType) {
			case Connection::ServerType:
				try {
					connection = connection->getLeafConnection(prefix);
				} catch (const std::exception& e) {
					throw std::invalid_argument("Invalid prefix");
				}
			case Connection::ClientType:
				break;
			case Connection::NoType:
				sendReplyToLocalClient(constructNotRegisteredReply(connection->client.nickName));
				throw std::invalid_argument("Bad user");
				break;
		}
		params.push_back(connection->client.nickName);
		params.push_back("TOPIC");
		server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NEEDMOREPARAMS, params));
		throw std::invalid_argument("Not enough params");
	}
	if (connection->connectionType == Connection::NoType) {
		server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
		throw std::invalid_argument("Client not registered");
	}
}

void ServerActionTopic::execute() {
	connection = server->getConnectionByFd(fd);
	std::string clientNick;
	try {
		if (connection->connectionType == Connection::ServerType) {
			Connection* tmp = connection->getLeafConnection(prefix);
			clientNick = tmp->client.nickName;
		} else {
			clientNick = connection->client.nickName;
		}
		chan = server->findChannel(params[0]);
	} catch (const ChannelException& e) {
		if (connection->connectionType == Connection::ClientType) {
			server->sendReplyToClient(fd, constructNoSuchChannelReply(clientNick, params[0]));
		}
	} catch (const std::exception& e) {
		Logger::log(LogLevelDebug, "Invalid prefix");
		return;
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
	if (modes.find('t') != std::string::npos
		&& connection->connectionType == Connection::ClientType
		&& !chan->isOper(connection)) {
		sendReplyToLocalClient(ReplyFactory::newReply(ERR_CHANOPRIVSNEEDED, replyParams));
	} else {
		chan->topic = params[1];
		chan->topicIsSet = true;
		std::vector<Connection*> broadcastTo = chan->getConnections();

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
		sendReplyToLocalClient(ReplyFactory::newReply(RPL_TOPIC, replyParams));
		if (chan->name[0] == '#') {
			if (connection->connectionType == Connection::ClientType) {
				server->sendMessageToAllServers(std::string(":" + sentFrom + " TOPIC " + chan->name + " :" + chan->topic + "\r\n"));
			} else if (connection->connectionType == Connection::ServerType) {
				server->sendMessageToAllServersButOne(std::string(":" + sentFrom + " TOPIC " + chan->name + " :" + chan->topic + "\r\n"), fd);
			}
		}
	}
}

void ServerActionTopic::checkTopic(const std::string& clientNick) const {
	std::vector<std::string> replyParams;
	replyParams.push_back(clientNick);
	replyParams.push_back(chan->name);

	if (chan->topic == "") {
		sendReplyToLocalClient(ReplyFactory::newReply(RPL_NOTOPIC, replyParams));
	} else {
		replyParams.push_back(chan->topic);
		sendReplyToLocalClient(ReplyFactory::newReply(RPL_TOPIC, replyParams));
	}
}

void ServerActionTopic::sendReplyToLocalClient(const std::string& message, const std::string& prefix) const {
	if (connection->connectionType == Connection::ClientType) {
		server->sendReplyToClient(fd, message, prefix);
	}
}

IServerAction* ServerActionTopic::clone() const {
	return (new ServerActionTopic(*this));
}
