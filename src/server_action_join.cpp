/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_join.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:17:13 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/09 16:52:04 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_join.h"

#include <vector>
#include <string>

#include "server.h"
#include "logger.h"
#include "utils.h"
#include "construct_reply.h"
#include "action_factory.h"

ServerActionJoin::ServerActionJoin(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionJoin::broadcastJoin(Channel* chan) {
    std::vector<Connection*> sendTo = chan->getConnections();
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
    if (chan->name[0] == '#') {
        if (connection->connectionType == Connection::ServerType) {
            server->sendMessageToAllServersButOne(std::string(":" + clientPrefix + " JOIN " + chan->name + " :" + chan->getKey() + "\r\n"), fd);
        } else if (connection->connectionType == Connection::ClientType) {
            server->sendMessageToAllServers(std::string(":" + clientPrefix + " JOIN " + chan->name + " :" + chan->getKey() + "\r\n"));
        }
    }
}

Connection* ServerActionJoin::getActualClient() {
	if (connection->connectionType == Connection::ServerType) {
		try {
			return (connection->getLeafConnection(prefix));
		} catch (const std::exception& e) {
			Logger::log(LogLevelDebug, "Cannot find leaf connection in JOIN");
			return (NULL);
		}
	} else if (connection->connectionType == Connection::ClientType) {
		return (connection);
	} else {
		Logger::log(LogLevelDebug, "connection type NoType in JOIN");
		return (NULL);
	}
}

void ServerActionJoin::sendNames(Channel* chan) {
	actionFactory factory;
	std::vector<std::string> namesParams;

	namesParams.push_back(chan->name);
	IServerAction* action = factory.newAction("NAMES", namesParams, fd, prefix);
	server->addNewAction(action);

}

void ServerActionJoin::addClientToChannel(const std::string& name, const std::string& key) {
    std::string reply;
    Channel* chan;
    std::vector<std::string> replyParams;

    replyParams.push_back(clientNick);
    replyParams.push_back(name);
	try {
        Connection* client;
		client = getActualClient();
        if (client == NULL) {
			return;
		}
		try {
            chan = server->findChannel(name);
            chan->addClient(client, key);
        } catch (const ChannelException& e) {
            if (connection->connectionType == Connection::ClientType) {
                server->sendReplyToClient(fd, e.what());
            }
            return;
        } catch (const std::exception &e) {
			chan = server->createNewChannel(name, client);
        }
        broadcastJoin(chan);
		sendNames(chan);
        if (chan->topicIsSet) {
            replyParams.push_back(chan->topic);
            reply = ReplyFactory::newReply(RPL_TOPIC, replyParams);
        } else {
            reply = ReplyFactory::newReply(RPL_NOTOPIC, replyParams);
        }
	} catch (const ChannelException& e) {
        reply = e.what();
    }
    sendToLocalClient(reply);
}

void ServerActionJoin::handleNeedMoreParams() const {
    std::vector<std::string> errParams;
    errParams.push_back(clientNick);
    errParams.push_back("JOIN");
    std::string errReply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, errParams);
    sendToLocalClient(errReply);
}

void ServerActionJoin::sendToLocalClient(const std::string& message, const std::string& prefix) const {
    if (connection->connectionType == Connection::ClientType) {
        server->sendReplyToClient(fd, message, prefix);
    }
}

void ServerActionJoin::execute() {
    Logger::log(LogLevelInfo, "server action join");
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType) {
        case Connection::ServerType:
            try {
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
            break;
    }
}

void ServerActionJoin::joinChannels() {
    if (params.size() < 1) {
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
            sendToLocalClient(e.what());
        }
    }
}

IServerAction* ServerActionJoin::clone() const {
    return (new ServerActionJoin(*this));
}
