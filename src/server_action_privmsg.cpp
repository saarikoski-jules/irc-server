/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_privmsg.cpp                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 13:44:06 by jules         #+#    #+#                 */
/*   Updated: 2021/05/20 14:08:05 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_privmsg.h"

#include "action_factory.h"
#include "server.h"
#include "construct_reply.h"
#include "utils.h"
#include "logger.h"
#include "reply.h"

ServerActionPrivmsg::ServerActionPrivmsg(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
    IServerAction(fd, 2, prefix),
    params(params) {
    if (params.size() < requiredParams) {
        Connection* connect = server->getConnectionByFd(fd);
        Client cli = connect->client;
        Logger::log(LogLevelDebug, "too few params");
        if (params.size() == 1) {
            server->sendReplyToClient(fd, constructNoTextToSendReply(cli.nickName));
        } else {
            server->sendReplyToClient(fd, constructNoRecipientReply(cli.nickName, "PRIVMSG"));
        }
        throw std::length_error("Bad amount of params for PRIVMSG");
    }
}

void ServerActionPrivmsg::execute() {
    Logger::log(LogLevelDebug, "execute PRIVMSG");
    std::vector<std::pair<Connection*, std::string> > sendTo;
    sender = server->getConnectionByFd(fd);
    switch (sender->connectionType) {
        case Connection::ServerType:
            //sender = sender->getLeafConnection(prefix);
        case Connection::ClientType:
            break;
        case Connection::NoType:
            connectionNotRegistered();
            return;
    }
    sendTo = findMatchingConnections();
    /* sendMessages(sendTo); */
}

std::vector<std::pair<Connection*, std::string> > ServerActionPrivmsg::findMatchingConnections() {
    std::vector<std::string> recipients = Utils::String::tokenize(params[0], params[0].length(), ",");
    std::vector<std::pair<Connection*, std::string> > sendTo;
	std::string broadcastTo;
	bool broadcast = false;

    for (std::vector<std::string>::iterator i = recipients.begin(); i != recipients.end(); i++) {
        try {
            if (std::string("$&#").find((*i)[0]) != std::string::npos) {
				if ((*i).find("#") != std::string::npos) {
					if (broadcastTo == "") {
						broadcastTo = std::string(*i);
					} else {
						broadcastTo = std::string(broadcastTo + "," + *i);
					}
					broadcast = true;
				}
                Channel* chan = server->findChannel(*i);
                //TODO(Jules): also look through host/server masks
                std::string channelModes = chan->getModes();
                if (channelModes.find('n') == std::string::npos || chan->connectionIsInChannel(sender)) {
                    std::vector<Connection*> channelClients = chan->getConnections();
                    for (std::vector<Connection*>::iterator cli = channelClients.begin(); cli != channelClients.end(); cli++) {
                        if (*cli != sender) {
							if (server->hasLocalConnection(**cli)) {
                            	sendTo.push_back(make_pair(*cli, *i));
							}
                        }
                    }
                } else {
                    if (sender->connectionType == Connection::ClientType) {
                        server->sendReplyToClient(fd, constructCannotSendToChanReply(sender->client.nickName, *i));
                    }
                }
            } else {
                Connection* cli = server->getClientByNick(*i);
                sendTo.push_back(make_pair(cli, cli->client.nickName));
            }
        } catch (const std::exception& e) {
            if (sender->connectionType == Connection::ClientType) {
                server->sendReplyToClient(fd, constructNoSuchNickReply(sender->client.nickName, *i));
            }
        }
    }
	sendMessages(sendTo, broadcast, broadcastTo);
    return (sendTo);
}

void ServerActionPrivmsg::sendMessages(const std::vector<std::pair<Connection*, std::string> >& recipients, bool broadcast, std::string targets) const {
    std::string senderPrefix;
	if (sender->connectionType == Connection::ServerType) {
		senderPrefix = prefix;
	} else {
		senderPrefix = sender->client.nickName;
	}
    for (std::vector<std::pair<Connection*, std::string> >::const_iterator cli = recipients.begin(); cli != recipients.end(); cli++) {
		std::string msg("PRIVMSG " + cli->second + " :" + params[1]);
		server->sendReplyToClient(cli->first->fd, msg, senderPrefix);
	}
	if (broadcast) {
		if (sender->connectionType == Connection::ClientType) {
			server->sendMessageToAllServers(std::string(":" + senderPrefix + " PRIVMSG " + targets + " :" + params[1]));
		} else if (sender->connectionType == Connection::ServerType) {
			server->sendMessageToAllServersButOne(std::string(":" + senderPrefix + " PRIVMSG " + targets + " :" + params[1]), fd);
		}
	}
}

void ServerActionPrivmsg::connectionNotRegistered() const {
    std::vector<std::string> params;
    params.push_back(sender->client.nickName);
    if (sender->connectionType == Connection::ClientType) {
        server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NOTREGISTERED, params));
    }
}

IServerAction* ServerActionPrivmsg::clone() const {
    return (new ServerActionPrivmsg(*this));
}

