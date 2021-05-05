/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_join.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:17:13 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/04 13:24:05 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_join.h"

#include <vector>
#include <string>

#include "server.h"
#include "logger.h"
#include "utils.h"

ServerActionJoin::ServerActionJoin(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

Channel* ServerActionJoin::getChannel(
    const std::string& name, const std::string& key) {
    Channel* chan;
    try {
        chan = server->findChannel(name);
    } catch (const std::exception &e) {
        chan = server->createNewChannel(name, fd);
    }
    chan->addClient(server->getConnectionByFd(fd), key);
    return (chan);
}

void ServerActionJoin::addClientToChannel(const std::string& name, const std::string& key) {
    std::string reply;
    Channel* chan;
    std::vector<std::string> replyParams;

    replyParams.push_back(connection->client.nickName);
    replyParams.push_back(name);
    try {
        chan = getChannel(name, key);
        if (chan->topicIsSet) {
            replyParams.push_back(chan->topic);
            reply = ReplyFactory::newReply(RPL_TOPIC, replyParams);
        } else {
            reply = ReplyFactory::newReply(RPL_NOTOPIC, replyParams);
        }
    } catch (const ChannelException& e) {
        reply = e.what();
    }
    server->sendReplyToClient(fd, reply);
}

void ServerActionJoin::handleNeedMoreParams() const {
    std::vector<std::string> errParams;
    Connection* connection = server->getConnectionByFd(fd);
    errParams.push_back(connection->client.nickName);
    errParams.push_back("JOIN");
    std::string errReply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, errParams);
    server->sendReplyToClient(fd, errReply);
}

void ServerActionJoin::execute() {
    Logger::log(LogLevelInfo, "server action join");
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType) {
        case Connection::ServerType:
            //idk what to do
            // addExternalClientToChannel();
            //find correct leaf connection, pass that to ServerActionJoin::connection
            try {
                //TODO(Jules): This is prefix stuff. Make sure prefixstuff works
                connection = connection->getLeafConnection(prefix);
            } catch (const std::exception& e) {
                // Connection not found. This should never happen?
            }
        case Connection::ClientType:
            joinChannels();
            break;
        
        case Connection::NoType:
            connectionNotRegistered();
            //error
            break;
    }
}

void ServerActionJoin::connectionNotRegistered() const {
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);//TODO(Jules): what nick should I use/where should I get it from
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NOTREGISTERED, params));
} 

void ServerActionJoin::joinChannels() {
    if (params.size() < 1) {    // TODO(Jules): handle need more params somewhere else
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
            server->sendReplyToClient(fd, e.what());
        }
    }
}

IServerAction* ServerActionJoin::clone() const {
    return (new ServerActionJoin(*this));
}
