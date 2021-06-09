/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_notice.cpp                          :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 13:44:06 by jules         #+#    #+#                 */
/*   Updated: 2021/06/09 14:03:48 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_notice.h"

#include "action_factory.h"
#include "server.h"
#include "construct_reply.h"
#include "utils.h"
#include "logger.h"
#include "reply.h"

ServerActionNotice::ServerActionNotice(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
    IServerAction(fd, 2, prefix),
    params(params) {
    if (params.size() < requiredParams) {
		// don't send error msg for notice
		throw std::length_error("Bad amount of params for PRIVMSG");
    }
}

void ServerActionNotice::execute() {
    std::vector<std::pair<Connection*, std::string> > sendTo;
    sender = server->getConnectionByFd(fd);
    switch (sender->connectionType) {
        case Connection::ServerType:
        case Connection::ClientType:
            break;
        case Connection::NoType:
            return;
    }
    findMatchingConnections();
}

void ServerActionNotice::findMatchingConnections() {
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
                	// No error msg sent on NOTICE
				}
            } else {
                Connection* cli = server->getClientByNick(*i);
                sendTo.push_back(make_pair(cli, cli->client.nickName));
            }
        } catch (const std::exception& e) {
			// No error msg sent on NOTICE
		}
    }
	sendMessages(sendTo, broadcast, broadcastTo);
}

void ServerActionNotice::sendMessages(const std::vector<std::pair<Connection*, std::string> >& recipients, bool broadcast, std::string targets) const {
    std::string senderPrefix;
	if (sender->connectionType == Connection::ServerType) {
		senderPrefix = prefix;
	} else {
		senderPrefix = sender->client.nickName;
	}
    for (std::vector<std::pair<Connection*, std::string> >::const_iterator cli = recipients.begin(); cli != recipients.end(); cli++) {
		std::string msg("NOTICE " + cli->second + " :" + params[1]);
		server->sendReplyToClient(cli->first->fd, msg, senderPrefix);
	}
	if (broadcast) {
		if (sender->connectionType == Connection::ClientType) {
			server->sendMessageToAllServers(std::string(":" + senderPrefix + " NOTICE " + targets + " :" + params[1]));
		} else if (sender->connectionType == Connection::ServerType) {
			server->sendMessageToAllServersButOne(std::string(":" + senderPrefix + " NOTICE " + targets + " :" + params[1]), fd);
		}
	}
}

IServerAction* ServerActionNotice::clone() const {
    return (new ServerActionNotice(*this));
}

