/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_names.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/05 11:40:59 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/18 12:33:03 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_names.h"

#include "utils.h"
#include "construct_reply.h"
#include "channel.h"
#include "server.h"
#include "reply.h"

ServerActionNames::ServerActionNames(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionNames::execute() {
	connection = server->getConnectionByFd(fd);
	switch (connection->connectionType) {
		case Connection::ServerType:
			return; // can't receive NAMES from server
		case Connection::ClientType:
			break;
		case Connection::NoType:
			server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
			return;
		
	}
	if (params.size() == 0) {
		std::map<std::string, Channel> channels = server->getListOfChannels();
		for (std::map<std::string, Channel>::iterator i = channels.begin(); i != channels.end(); i++) {
			try {
				std::string names = (*i).second.getNames(connection);
				namesReply((*i).second.name, names);
			} catch (const ChannelException& e) {
				//do non
			}
		}
		endOfNamesReply("*");
		//send all names after which only one end of names reply
	} else {
		std::vector<std::string> channels = Utils::String::tokenize(params[0], params[0].length(), ",");
		for (std::vector<std::string>::iterator i = channels.begin(); i != channels.end(); i++) {
			try {
				Channel* chan = server->findChannel(*i);
				std::string names = chan->getNames(connection);
				namesReply(chan->name, names);
			} catch (const std::exception& e) {
				//channel not found. No numeric reply?
			}
			//send end of names reply
			endOfNamesReply(*i);
		}
	}
}

void ServerActionNames::namesReply(const std::string& channelName, const std::string& names) const {
	std::vector<std::string> params;

	params.push_back(connection->client.nickName);
	params.push_back(channelName);
	params.push_back(names);
	server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_NAMREPLY, params));
}

void ServerActionNames::endOfNamesReply(const std::string& channelName) const {
	std::vector<std::string> params;

	params.push_back(connection->client.nickName);
	params.push_back(channelName);
	server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_ENDOFNAMES, params));
}

IServerAction* ServerActionNames::clone() const {
	return (new ServerActionNames(*this));
}
