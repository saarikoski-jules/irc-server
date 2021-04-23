#include "channel.h"

#include <string>
#include <vector>

#include "connection.h"
#include "client.h"
#include "reply.h"

Channel::Channel(const std::string& name, Connection* chanop) :
name(name),
topicIsSet(false),
chanop(chanop),
connections() {
    if ((name[0] != '&' && name[0] != '#') || name.length() > 200) {
        std::vector<std::string> replyParams;
        replyParams.push_back(this->chanop->client.nickName);
        replyParams.push_back(name);
        std::string reply = ReplyFactory::newReply(ERR_NOSUCHCHANNEL, replyParams);
        throw ChannelException(reply, false);
    }
}

void Channel::addClient(Connection* connection, const std::string& key) {
    if (this->key == "" || this->key == key) {
        connections.push_back(connection);
    } else {
        std::vector<std::string> errorParams;
        errorParams.push_back(connection->client.nickName);
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

