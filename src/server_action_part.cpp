/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_part.cpp                            :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/20 16:04:13 by jules         #+#    #+#                 */
/*   Updated: 2021/06/09 17:28:50 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_part.h"

#include <string>
#include <vector>

#include "construct_reply.h"
#include "utils.h"
#include "server.h"
#include "logger.h"

ServerActionPart::ServerActionPart(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {
	connection = server->getConnectionByFd(fd);
	if (params.size() < requiredParams) {
		if (server->hasLocalConnection(*connection)) {
			server->sendReplyToClient(fd, constructNeedMoreParamsReply(connection->client.nickName, "PART"));
		}
		throw std::invalid_argument("Not enough params");
	}
	if (connection->connectionType == Connection::NoType) {
		server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
		throw std::invalid_argument("Client not registered");
	}
}

void ServerActionPart::broadcastPart() const {
	std::vector<Connection*> sendTo = chan->getConnections();
	std::string senderPrefix;
	if (connection->connectionType == Connection::ClientType) {
		senderPrefix = std::string(connection->client.nickName + "!" + connection->client.userName + "@" + connection->client.hostName);
	} else {
		senderPrefix = prefix;
	}

	for (std::vector<Connection*>::iterator it = sendTo.begin(); it != sendTo.end(); it++) {
		if (server->hasLocalConnection(**it)) {
			server->sendReplyToClient((*it)->fd, std::string("PART " + chan->name), senderPrefix);
		}
	}
	std::string msg(":" + senderPrefix + " PART " + chan->name + "\r\n");
	if (chan->name[0] == '#') {
		if (connection->connectionType == Connection::ClientType) {
			server->sendMessageToAllServers(msg);
		} else {
			server->sendMessageToAllServersButOne(msg, fd);
		}
	}
}

void ServerActionPart::execute() {
	Logger::log(LogLevelDebug, "PART action");
	std::vector<std::string> channels = Utils::String::tokenize(params[0], params[0].length(), ",");
	for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); it++) {
		Logger::log(LogLevelDebug, std::string("parting " + *it));
		try {
			chan = server->findChannel(*it);
			Connection* tmp;
			if (connection->connectionType == Connection::ServerType) {
				tmp = connection->getLeafConnection(prefix);
			} else {
				tmp = connection;
			}
			broadcastPart();
			chan->removeConnection(tmp);
			if (chan->getAmtUsers() == 0) {
				server->deleteChannel(chan);
			}
		} catch (const ChannelException& e) {
			if (connection->connectionType == Connection::ClientType) {
				server->sendReplyToClient(fd, constructNotOnChannelReply(connection->client.nickName, *it));
			}
		} catch (const std::exception& e) {
			if (connection->connectionType == Connection::ClientType) {
				server->sendReplyToClient(fd, constructNoSuchChannelReply(connection->client.nickName, *it));
			}
		}
	}
}

IServerAction* ServerActionPart::clone() const {
	return (new ServerActionPart(*this));
}
