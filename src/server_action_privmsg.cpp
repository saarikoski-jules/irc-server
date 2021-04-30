/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_privmsg.cpp                         :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/04/28 13:44:06 by jules        #+#    #+#                  */
/*   Updated: 2021/04/30 14:12:31 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_privmsg.h"

#include "action_factory.h"
#include "server.h"
#include "construct_reply.h"

ServerActionPrivmsg::ServerActionPrivmsg(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
    IServerAction(fd, 2, prefix),
    params(params) {
    if (params.size() < requiredParams) {
        Connection* connect = server->getConnectionByFd(fd);
        Client cli = connect->client;
        server->sendReplyToClient(fd, constructNeedMoreParamsReply(cli.nickName, "PRIVMSG"));
    }
}

void ServerActionPrivmsg::execute() {
    for (std::vector<std::string>::iterator i = params.begin(); i != params.end() - 1; i++) {
        try {
            Connection* cli = server->getClientByNick(*i);
            server->sendReplyToClient(cli->fd, *(params.end() - 1));
        } catch (const std::exception& e) {
            // no such client
            try {
                Channel* chan = server->findChannel(*i);
                chan->sendToAllConnections(*(params.end() - 1));
            } catch (const std::exception& e) {
                //no such channel
            }
        }
    }
}

IServerAction* ServerActionPrivmsg::clone() const {
    return (new ServerActionPrivmsg(*this));
}
