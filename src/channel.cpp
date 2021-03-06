/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   channel.cpp                                       :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 14:18:48 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/02 15:54:42 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "channel.h"

#include <string>
#include <vector>
#include <algorithm>

#include "connection.h"
#include "client.h"
#include "reply.h"
#include "logger.h"
#include "int_conversions.h"
#include "construct_reply.h"

Channel::Channel(const std::string& name, Connection* chanop) :
name(name),
topicIsSet(false),
bans(),
chanops(),
connections(),
key(""),
modes("") {
    if ((name[0] != '&' && name[0] != '#') || name.length() > 200 || name.find(' ') != std::string::npos) {
        std::vector<std::string> replyParams;
        replyParams.push_back(chanop->client.nickName);
        replyParams.push_back(name);
        std::string reply = ReplyFactory::newReply(ERR_NOSUCHCHANNEL, replyParams);
        throw ChannelException(reply, false);
    }
    chanops.push_back(chanop);
    connections.push_back(chanop);
}

void Channel::addOperator(Connection* newChanop) {
	if (!connectionIsInChannel(newChanop)) {
		throw ChannelException("user not in channel", false);
	}
	if (!isOper(newChanop)) {
		chanops.push_back(newChanop);
	}
}

void Channel::removeOperator(Connection* target) {
    std::vector<Connection*>::iterator found = std::find(chanops.begin(), chanops.end(), target);
    if (found == chanops.end()) {
        throw ChannelException("Channel exception: No such chanop", false);
    } else {
        chanops.erase(found);
    }
}

void Channel::setLimit(unsigned int lim) {
    this->limit = lim;
    addMode('l');
}

unsigned int Channel::getLimit() const {
	return (limit);
}

void Channel::changeKey(const std::string& key) {
    this->key = key;
}

std::string Channel::getKey() const {
	return (key);
}

void Channel::removeMode(char c) {
    size_t pos = modes.find(c);
    if (pos != std::string::npos) {
        modes.erase(pos, 1);
    }
}

void Channel::addMode(char c) {
    if (modes.find(c) == std::string::npos) {
        modes.push_back(c);
    }
}

std::string Channel::getModes() const {
    return (modes);
}

size_t Channel::getAmtUsers() const {
	return (connections.size());
}

bool Channel::isBanned(Client* client) const {
    for (std::vector<std::string>::const_iterator i = bans.begin(); i != bans.end(); i++) {
        size_t posUser = (*i).find('!') + 1;
        size_t posHost = (*i).find('@') + 1;
        std::string maskNick(*i, 0, posUser - 1);
        std::string maskUser(*i, posUser, posHost - posUser - 1);
        std::string maskHost(*i, posHost, i->length() - posHost);

        Logger::log(LogLevelDebug,
        std::string("maskNick: " + maskNick + " client nick: " + client->nickName));
        Logger::log(LogLevelDebug,
            std::string("maskUser: " + maskUser + " client user: " + client->userName));
        Logger::log(LogLevelDebug,
            std::string("maskHost: " + maskHost + " client host: " + client->hostName));
        if ((client->nickName == maskNick || maskNick == "*")
        && (client->userName == maskUser || maskUser == "*")
        && (client->hostName == maskHost || maskHost == "*")) {
            return (true);
        }
    }
    return (false);
}

bool Channel::canJoin(Client* client, const std::string& key) const {
    if (modes.find('k') != std::string::npos && this->key != key) {
        throw 'k';
		return (false);
    }
    if (modes.find('l') != std::string::npos && limit <= connections.size()) {
		throw 'l';
		return (false);
    }
    if (isBanned(client)) {
        throw 'b';
		return (false);
    }
    return (true);
}

void Channel::addClient(Connection* connection, const std::string& key) {
    Client* client = &connection->client;
    try {
        canJoin(client, key);
		connections.push_back(connection);
    } catch (const char& e) {
        std::vector<std::string> errorParams;
        errorParams.push_back(connection->client.nickName);
        errorParams.push_back(name);
		errorParams.push_back(std::string(1, e));
        throw ChannelException(ReplyFactory::newReply(ERR_BADCHANNELKEY, errorParams), false);
    }
}

void Channel::removeConnection(const Connection* toRemove) {
	std::vector<Connection*>::iterator pos = std::find(chanops.begin(), chanops.end(), toRemove);
	if (pos != chanops.end()) {
		chanops.erase(pos);
	}
	pos = std::find(connections.begin(), connections.end(), toRemove);
	if (pos == connections.end()) {
		throw ChannelException("Client not in channel", false);
	}
	connections.erase(pos);
}

void Channel::addBanMask(const std::string& mask) {
    bans.push_back(mask);
}

void Channel::removeBanMask(const std::string& mask) {
    std::vector<std::string>::iterator pos = std::find(bans.begin(), bans.end(), mask);
    if (pos != bans.end()) {
        bans.erase(pos);
    }
}

std::string Channel::getBanMask(size_t index) const {
    if (this->bans.size() > index) {
        return (bans[index]);
    } else {
        throw std::out_of_range("index not present in masks");
    }
}

std::string Channel::getNames(Connection* connection) const {
    std::string names;

    clientHasAccess(connection);
    for (std::vector<Connection*>::const_iterator user = chanops.begin(); user != chanops.end(); user++) {
        names = names + "@" + (*user)->client.nickName + " ";
    }
    for (std::vector<Connection*>::const_iterator user = connections.begin(); user != connections.end(); user++) {
        if (!isOper(*user)) {
            names = names + (*user)->client.nickName + " ";
        }
    }
    return (names);
}

std::string Channel::getChannelModes() const {
	std::string modeStr = std::string("+" + getModes());
	std::vector<std::string> modeParams;

	for (std::string::iterator it = modeStr.begin(); it != modeStr.end(); it++) {
		if (*it == 'l') {
			modeParams.push_back(IntConversion::intToString(limit));
		}
		if (*it == 'k') {
			modeParams.push_back(key);
		}
	}

	std::string reply = constructChannelModeIs(name, modeStr, modeParams);
	return (reply);
}

void Channel::clientHasAccess(Connection* connection) const {
    if (modes.find('s') != std::string::npos || modes.find('p') != std::string::npos) {
		std::vector<Connection *>::const_iterator pos = std::find(connections.begin(), connections.end(), connection);
		if (pos == connections.end()) {
			throw ChannelException("Channel not visible to user", false);
		} else {
			return;
		}
    }
}

bool Channel::isOper(const Connection* connection) const {
    for (std::vector<Connection*>::const_iterator oper = chanops.begin(); oper != chanops.end(); oper++) {
        if (connection->client.nickName == (*oper)->client.nickName) {
            return (true);
        }
    }
    return (false);
}

bool Channel::connectionIsInChannel(const Connection* connection) const {
	std::vector<Connection*>::const_iterator it = std::find(connections.begin(), connections.end(), connection);
	if (it == connections.end()) {
		return (false);
	}
	return (true);
}

std::vector<Connection*> Channel::getConnections() const {
    return (connections);
}

ChannelException::ChannelException(const std::string& what, const bool& fatal) :
fatal(fatal),
message(what) {
    if (isFatal()) {
        fullMessage = std::string("Fatal channel exception: " + message);
    } else {
        fullMessage = std::string(message);
    }
}

ChannelException::~ChannelException() throw() {
}

const bool& ChannelException::isFatal() const {
    return (fatal);
}

const char* ChannelException::what() const throw() {
    return (fullMessage.c_str());
}
