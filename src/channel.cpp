#include "channel.h"

#include <string>
#include <vector>
#include <algorithm>

#include "client.h"
#include "reply.h"

Channel::Channel(const std::string& name, Client* chanop) :
name(name),
topicIsSet(false),
chanops(),
clients(),
bans(),
key(""),
modes("") {
    if ((name[0] != '&' && name[0] != '#') || name.length() > 200) {
        std::vector<std::string> replyParams;
        replyParams.push_back(chanop->nickName);
        replyParams.push_back(name);
        std::string reply = ReplyFactory::newReply(ERR_NOSUCHCHANNEL, replyParams);
        throw ChannelException(reply, false);
    }
    addOperator(chanop);
}

void Channel::addOperator(Client* newChanop) {
    chanops.push_back(newChanop);
}

bool Channel::isOperator(Client* cli) const {
    if (std::find(chanops.begin(), chanops.end(), cli) == chanops.end()) {
        return (false);
    }
    return (true);
}

void Channel::removeOperator(Client* target) {
    std::vector<Client*>::iterator found = std::find(chanops.begin(), chanops.end(), target);
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


//TODO: check if +k "" sets password to no password
//secret can't display anything, and you can't tell someone is in the channel
//private shows up in channel listings, but you can't tell a user is in it
//TODO: -k doesn't unset password
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

#include <iostream>
bool Channel::isBanned(Client* client) const {
    for (std::vector<std::string>::const_iterator i = bans.begin(); i != bans.end(); i++) {
        size_t posUser = (*i).find('!') + 1;
        size_t posHost = (*i).find('@') + 1;
        std::string maskNick(*i, 0, posUser - 1);
        std::string maskUser(*i, posUser, posHost - posUser);
        std::string maskHost(*i, posHost, i->length() - posHost);

        std::cout << "maskNick: " << maskNick << " client nick: " << client->nickName << std::endl;
        std::cout << "maskUser: " << maskUser << " client user: " << client->userName << std::endl;
        std::cout << "maskHost: " << maskHost << " client host: " << client->hostName << std::endl;
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
    if (modes.find('l') != std::string::npos && limit >= clients.size()) {
        return (false);
    }
    if (isBanned(client)) {
        return (false);
    }
    // if user is banned
    return (true);
}

void Channel::addClient(Client* client, const std::string& key) {
    if (canJoin(client, key)) {
        clients.push_back(client);
    } else {
        std::vector<std::string> errorParams;
        errorParams.push_back(client->nickName);
        errorParams.push_back(name);
        throw ChannelException(ReplyFactory::newReply(ERR_BADCHANNELKEY, errorParams), false);
    }
}
void Channel::addBanMask(const std::string& mask) {
    bans.push_back(mask);
}

void Channel::removeBanMask(const std::string& mask) {//correct, should remove banmask
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

ChannelException::ChannelException(const std::string& what, const bool& fatal) :
fatal(fatal),
message(what) {
}

ChannelException::~ChannelException() throw() {
}

const bool& ChannelException::isFatal() const {
    return (fatal);
}

const char* ChannelException::what() const throw() {
    if (isFatal()) {
        return (std::string("Fatal channel exception: " + message).c_str());
    }
    return (message.c_str());
}
