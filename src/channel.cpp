#include "channel.h"

#include <string>
#include <vector>

#include "client.h"
#include "reply.h"

Channel::Channel(const std::string& name, Client* chanop) :
name(name),
topicIsSet(false),
chanop(chanop),
clients() {
    if ((name[0] != '&' && name[0] != '#') || name.length() > 200) {
        std::vector<std::string> replyParams;
        replyParams.push_back(name);
        std::string reply = ReplyFactory::newReply(ERR_NOSUCHCHANNEL, replyParams);//TODO(Jules): is this the appropriate error message?
        throw ChannelException(reply, false);
    }
}

void Channel::addClient(Client* client, const std::string& key) {
    (void)chanop;//TODO: remove
    if (this->key == "" || this->key == key) {
        clients.push_back(client);
    } else {
        std::vector<std::string> errorParams;
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
    return (std::string("Channel exception: " + message).c_str());
}
