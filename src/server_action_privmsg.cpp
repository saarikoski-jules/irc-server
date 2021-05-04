/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_privmsg.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 13:44:06 by jules         #+#    #+#                 */
/*   Updated: 2021/05/04 15:03:00 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_privmsg.h"

#include "action_factory.h"
#include "server.h"
#include "construct_reply.h"
#include "utils.h"
#include "logger.h"
#include "reply.h"

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

void ServerActionPrivmsg::execute() {
    std::vector<std::pair<Connection*, std::string> > sendTo;
    sender = server->getConnectionByFd(fd);
    switch (sender->connectionType) {
        case Connection::ServerType:
            //sender = sender->getLeafConnection(prefix);
        case Connection::ClientType:
            break;
        case Connection::NoType:
            connectionNotRegistered();
            return;
    }
    sendTo = findMatchingConnections();
    sendMessages(sendTo);
}

std::vector<std::pair<Connection*, std::string> > ServerActionPrivmsg::findMatchingConnections() {
    std::vector<std::string> recipients = Utils::String::tokenize(params[0], params[0].length(), ",");
    std::vector<std::pair<Connection*, std::string> > sendTo;
    for (std::vector<std::string>::iterator i = recipients.begin(); i != recipients.end(); i++) {
        try {
            if (std::string("$&#").find((*i)[0]) != std::string::npos) {
                Channel* chan = server->findChannel(*i);
                //TODO(Jules): also look through host/server masks
                std::vector<Connection*> channelClients = chan->getConnections(*sender);
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
        } catch (const ChannelException& e) {
            server->sendReplyToClient(fd, constructCannotSendToChanReply(sender->client.nickName, *i));
        } catch (const std::exception& e) {
            server->sendReplyToClient(fd, constructNoSuchNickReply(sender->client.nickName, *i));
            // no such client
        }
    }
    return (sendTo);
}

void ServerActionPrivmsg::sendMessages(const std::vector<std::pair<Connection*, std::string> >& recipients) const {
    std::string prefix = sender->client.nickName; //TODO(Jules): This also needs to work with server? Should also append user's server/name/etc?
    for (std::vector<std::pair<Connection*, std::string> >::const_iterator cli = recipients.begin(); cli != recipients.end(); cli++) {
        std::string msg("PRIVMSG " + cli->second + " :" + params[1]);
        server->sendReplyToClient(cli->first->fd, msg, prefix);
    }
}

void ServerActionPrivmsg::connectionNotRegistered() const {
    std::vector<std::string> params;
    params.push_back(sender->client.nickName);
    server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NOTREGISTERED, params));
}

IServerAction* ServerActionPrivmsg::clone() const {
    return (new ServerActionPrivmsg(*this));
}
