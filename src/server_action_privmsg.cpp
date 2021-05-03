/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_privmsg.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 13:44:06 by jules         #+#    #+#                 */
/*   Updated: 2021/05/03 12:58:52 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_privmsg.h"

#include "action_factory.h"
#include "server.h"
#include "construct_reply.h"
#include "utils.h"
#include "logger.h"

ServerActionPrivmsg::ServerActionPrivmsg(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
    IServerAction(fd, 2, prefix),
    params(params) {
    if (params.size() < requiredParams) {
        Connection* connect = server->getConnectionByFd(fd);
        Client cli = connect->client;
        std::string reply = constructNeedMoreParamsReply(cli.nickName, "PRIVMSG");
        Logger::log(LogLevelDebug, "too few params");
        Logger::log(LogLevelDebug, reply);
        if (params.size() == 1) {
            server->sendReplyToClient(fd, constructNoTextToSendReply(cli.nickName));
        } else {
            server->sendReplyToClient(fd, constructNoRecipientReply(cli.nickName, "PRIVMSG"));
        }
        throw std::length_error("Bad amount of params for PRIVMSG");
    }
}
#include <iostream>
void ServerActionPrivmsg::execute() {
    std::vector<std::pair<Connection*, std::string> > sendTo;
    Connection* sender = server->getConnectionByFd(fd);
    std::vector<std::string> recipients = Utils::String::tokenize(params[0], params[0].length(), ",");
    std::cout << "in privmsg: param 0: " << params[0] << std::endl;
    std::cout << "in privmsg: param 1: " << params[0] << std::endl;
    for (std::vector<std::string>::iterator i = recipients.begin(); i != recipients.end(); i++) {
        std::cout << "recipient: " << *i << std::endl;
        std::cout << "msg: " << params[1];
        try {
            if (std::string("$&#").find((*i)[0]) != std::string::npos) {
                Channel* chan = server->findChannel(*i);
                //TODO(Jules): also look through host/server masks
                std::vector<Connection*> channelClients = chan->getConnections();
                for (std::vector<Connection*>::iterator cli = channelClients.begin(); cli != channelClients.end(); cli++) {
                    if (*cli != sender) {
                        sendTo.push_back(make_pair(*cli, *i));
                    }
                }
            } else if (i->find('@') != std::string::npos) {
                //TODO(Jules): find by user@host
            } else {
                Connection* cli = server->getClientByNick(*i);
                sendTo.push_back(make_pair(cli, cli->client.nickName));
            }
        } catch (const std::exception& e) {
            server->sendReplyToClient(fd, constructNoSuchNickReply(sender->client.nickName, *i));
            // no such client
        }
    }
    std::string prefix = sender->client.nickName; //TODO(Jules): This also needs to work with server? Should also append user's server/name/etc?
    for (std::vector<std::pair<Connection*, std::string> >::iterator cli = sendTo.begin(); cli != sendTo.end(); cli++) {
        std::string msg("PRIVMSG " + cli->second + " :" + params[1]);
        server->sendReplyToClient(cli->first->fd, msg, prefix);
    }
}

IServerAction* ServerActionPrivmsg::clone() const {
    return (new ServerActionPrivmsg(*this));
}
