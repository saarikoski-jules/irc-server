/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_join.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:17:13 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 11:25:44 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_join.h"

#include "server.h"
#include "logger.h"
#include "utils.h"

ServerActionJoin::ServerActionJoin(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

Channel* ServerActionJoin::getChannel(
    const std::string& name, const std::string& key) {
    Channel* chan;
    try {
        chan = server->findChannel(name);
    } catch (const std::exception &e) {
        chan = server->createNewChannel(name, clientFd);
    }
    chan->addClient(server->getClientByFd(clientFd), key);
    return (chan);
}

void ServerActionJoin::joinServer(const std::string& name, const std::string& key) {
    std::string reply;
    Channel* chan;
    std::vector<std::string> replyParams;

    replyParams.push_back(cli->nickName);
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
    server->sendReplyToClient(clientFd, reply);
}

void ServerActionJoin::handleNeedMoreParams() const {
    std::vector<std::string> errParams;
    errParams.push_back(cli->nickName);
    errParams.push_back("JOIN");
    std::string errReply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, errParams);
    server->sendReplyToClient(clientFd, errReply);
}

void ServerActionJoin::execute() {
    Logger::log(LogLevelInfo, "server action join");
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
            joinServer(chans[i], key);
        } catch (const ChannelException& e) {
            server->sendReplyToClient(clientFd, e.what());
        }
    }
}
