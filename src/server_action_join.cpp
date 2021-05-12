/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_join.cpp                            :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:17:13 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/12 16:39:44 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_join.h"

#include <vector>
#include <string>

#include "server.h"
#include "logger.h"
#include "utils.h"
#include "construct_reply.h"

ServerActionJoin::ServerActionJoin(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

Channel* ServerActionJoin::getChannel(
    const std::string& name, const std::string& key) {
    Channel* chan;
    try {
        chan = server->findChannel(name);
    } catch (const std::exception &e) {
        chan = server->createNewChannel(name, fd);
    }
    chan->addClient(server->getConnectionByFd(fd), key);
    return (chan);
}

void ServerActionJoin::addClientToChannel(const std::string& name, const std::string& key) {
    std::string reply;
    Channel* chan;
    std::vector<std::string> replyParams;

    replyParams.push_back(clientNick);
    replyParams.push_back(name);
    try {
        chan = getChannel(name, key);
		std::vector<Connection*> sendTo = chan->getConnections(*connection);
		std::string clientPrefix;
		if (connection->connectionType == Connection::ServerType) {
			clientPrefix = prefix;
		} else if (connection->connectionType == Connection::ClientType) {
			clientPrefix = std::string(connection->client.nickName + "!" + connection->client.userName + "@" + connection->client.hostName);
		}
		for (std::vector<Connection*>::iterator it = sendTo.begin(); it != sendTo.end(); it++) {
			if (server->hasLocalConnection(**it)) {
				server->sendReplyToClient((*it)->fd, std::string("JOIN :" + chan->name), clientPrefix);
			}
		}
		if (chan->name == "#") {
			if (connection->connectionType == Connection::ServerType) {
				server->sendMessageToAllServersButOne(std::string(":" + clientPrefix + "JOIN :" + chan->name), fd);
			} else if (connection->connectionType == Connection::ClientType) {
				server->sendMessageToAllServers(std::string(":" + clientPrefix + "JOIN :" + chan->name));
			}
		}
        if (chan->topicIsSet) {
            replyParams.push_back(chan->topic);
            reply = ReplyFactory::newReply(RPL_TOPIC, replyParams);
        } else {
            reply = ReplyFactory::newReply(RPL_NOTOPIC, replyParams);
        }
	} catch (const ChannelException& e) {
        reply = e.what();
    }
    server->sendReplyToClient(fd, reply);
}

void ServerActionJoin::handleNeedMoreParams() const {
    std::vector<std::string> errParams;
    errParams.push_back(clientNick);
    errParams.push_back("JOIN");
    std::string errReply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, errParams);
    server->sendReplyToClient(fd, errReply);
}

void ServerActionJoin::execute() {
    Logger::log(LogLevelInfo, "server action join");
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType) {
        case Connection::ServerType:
            //idk what to do
            // addExternalClientToChannel();
            //find correct leaf connection, pass that to ServerActionJoin::connection
            try {
                //TODO(Jules): This is prefix stuff. Make sure prefixstuff works
                Connection* tmp = connection->getLeafConnection(prefix);
				clientNick = tmp->client.nickName;
            } catch (const std::exception& e) {
                // Connection not found. This should never happen?
            }
			joinChannels();	
			break;
		case Connection::ClientType:
			clientNick = connection->client.nickName;
            joinChannels();
            break;
        
        case Connection::NoType:
            server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
            //error
            break;
    }
}

void ServerActionJoin::joinChannels() {
    if (params.size() < 1) {    // TODO(Jules): handle need more params somewhere else
        handleNeedMoreParams();
        return;
    }
    std::vector<std::string> chans = Utils::String::tokenize(params[0], params[0].length(), ",");
    std::vector<std::string> keys;
    if (params.size() > 1) {
        keys = Utils::String::tokenize(params[1], params[1].length(), ",");
    }
    for (size_t i = 0; i < chans.size(); i++) {
        std::string key;
        if (keys.size() > i) {
            key = keys[i];
        } else {
            key = "";
        }
        try {
            addClientToChannel(chans[i], key);
        } catch (const ChannelException& e) {
            server->sendReplyToClient(fd, e.what());
        }
    }
}

IServerAction* ServerActionJoin::clone() const {
    return (new ServerActionJoin(*this));
}
