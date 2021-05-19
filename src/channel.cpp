/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   channel.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 14:18:48 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/18 15:21:11 by jsaariko      ########   odam.nl         */
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

Channel::Channel(const std::string& name, Connection* chanop) :
name(name),
topicIsSet(false),
bans(),
chanops(),
connections(),
key(""),
modes("") {
    if ((name[0] != '&' && name[0] != '#') || name.length() > 200) {
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
    chanops.push_back(newChanop);
    //TODO: Do i need to make sure chanop is also already a connection in channel?
}

bool Channel::isOperator(Connection* cli) const {
    if (std::find(chanops.begin(), chanops.end(), cli) == chanops.end()) {
        return (false);
    }
    return (true);
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

void Channel::changeKey(const std::string& key) {
    this->key = key;
}

// TODO(Jules): secret can't display anything, and you can't tell someone is in the channel
// TODO(Jules): private shows up in channel listings, but you can't tell a user is in it
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

// TODO(Jules): write a matching function for partial masks
bool Channel::isBanned(Client* client) const {
    for (std::vector<std::string>::const_iterator i = bans.begin(); i != bans.end(); i++) {
        size_t posUser = (*i).find('!') + 1;
        size_t posHost = (*i).find('@') + 1;
        std::string maskNick(*i, 0, posUser - 1);
        std::string maskUser(*i, posUser, posHost - posUser);
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
        return (false);
    }
    // if (modes.find('i') && client has no invite)
    if (modes.find('l') != std::string::npos && limit <= connections.size()) {
        return (false);
    }
    if (isBanned(client)) {
        return (false);
    }
    return (true);
}

void Channel::addClient(Connection* connection, const std::string& key) {
    Client* client = &connection->client;
    if (canJoin(client, key)) {
        connections.push_back(connection);
    } else {
        std::vector<std::string> errorParams;
        errorParams.push_back(connection->client.nickName);
        errorParams.push_back(name);
        throw ChannelException(ReplyFactory::newReply(ERR_BADCHANNELKEY, errorParams), false);
    }
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
        Logger::log(LogLevelDebug, (*user)->client.nickName);
        Logger::log(LogLevelDebug, names);
    }
    for (std::vector<Connection*>::const_iterator user = connections.begin(); user != connections.end(); user++) {
        if (!isOper(*user)) {
            names = names + "+" + (*user)->client.nickName + " ";
        }
    }
    return (names);
}

void Channel::clientHasAccess(Connection* connection) const {
    if (modes.find('s') != std::string::npos || modes.find('p') != std::string::npos) {
        for (std::vector<Connection*>::const_iterator i = connections.begin(); i != connections.end(); i++) {
            if (i + 1 == connections.end()) {
                throw ChannelException("Channel not visible to user", false);
            }
            if ((*i)->client.nickName == connection->client.nickName) {
                return;
            }
        }
    }
}

bool Channel::isOper(Connection* connection) const {
    for (std::vector<Connection*>::const_iterator oper = chanops.begin(); oper != chanops.end(); oper++) {
        if (connection->client.nickName == (*oper)->client.nickName) {
            return (true);
        }
    }
    return (false);
}

bool Channel::connectionIsInChannel(Connection* connection) const {
    for (std::vector<Connection*>::const_iterator oper = connections.begin(); oper != connections.end(); oper++) {
        if (connection->client.nickName == (*oper)->client.nickName) {
            return (true);
        }
    }
    return (false);
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
        fullMessage = std::string("Channel exception: " + message);
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
