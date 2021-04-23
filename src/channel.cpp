/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   channel.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/23 12:35:03 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 12:35:04 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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

