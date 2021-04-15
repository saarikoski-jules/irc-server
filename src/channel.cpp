#include "channel.h"

#include <string>
#include <vector>

#include "client.h"
#include "reply.h"

Channel::Channel(const std::string& name, Client* chanop) :
name(name),
topicIsSet(false),
chanops(),
clients(),
key(""),
modes("") {
    if ((name[0] != '&' && name[0] != '#') || name.length() > 200) {
        std::vector<std::string> replyParams;
        replyParams.push_back(chanop->nickName);
        replyParams.push_back(name);
        std::string reply = ReplyFactory::newReply(ERR_NOSUCHCHANNEL, replyParams);
        throw ChannelException(reply, false);//I can't do this
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

std::string Channel::getModeParams() const {
    std::string params = "mode params?";

    return (params);
}

void Channel::addClient(Client* client, const std::string& key) {
    if (this->key == "" || this->key == key) {
        clients.push_back(client);
    } else {
        std::vector<std::string> errorParams;
        errorParams.push_back(client->nickName);
        errorParams.push_back(name);
        throw ChannelException(ReplyFactory::newReply(ERR_BADCHANNELKEY, errorParams), false);
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

