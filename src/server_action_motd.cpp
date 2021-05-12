/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_motd.cpp                            :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/07 12:05:23 by jules        #+#    #+#                  */
/*   Updated: 2021/05/12 11:01:40 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_motd.h"

#include <fcntl.h>
#include <unistd.h>

#include "server.h"
#include "construct_reply.h"
#include "utils.h"

ServerActionMotd::ServerActionMotd(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

std::string constructNotRegisteredReply(const std::string& nick) {
    std::vector<std::string> params;
    params.push_back(nick);
    std::string reply = ReplyFactory::newReply(ERR_NOTREGISTERED, params);
    return (reply);
}

void ServerActionMotd::execute() {
   Connection* connection = server->getConnectionByFd(fd);
   switch(connection->connectionType) {
        case Connection::ServerType:
            connection = connection->getLeafConnection(prefix);//TODO(Jules): will this work?
        case Connection::ClientType:
            break;
        case Connection::NoType:
            server->sendReplyToClient(fd, constructNotRegisteredReply(connection->client.nickName));
            return;
    }
    std::vector<std::string> params;
    params.push_back(connection->client.nickName);
    std::string reply;
    reply = ReplyFactory::newReply(RPL_MOTDSTART, params);
    server->sendReplyToClient(fd, reply);

    char readBuf[10000];
    int motdFile = open("conf/irc.motd", O_RDONLY);
    int amtRead = read(motdFile, readBuf, 10000);
    if (amtRead > 0) {
        std::vector<std::string> lines = Utils::String::tokenize(std::string(readBuf), 1000, "\n");
        params.push_back("aa");
        for (std::vector<std::string>::iterator i = lines.begin(); i != lines.end(); i++) {
            params.pop_back();
            params.push_back(*i);
            reply = ReplyFactory::newReply(RPL_MOTD, params);
            server->sendReplyToClient(fd, reply);
        }
    } else {
        server->sendReplyToClient(fd, ReplyFactory::newReply(ERR_NOMOTD, params));
    }
    close(motdFile);
    server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_ENDOFMOTD, params));
}

IServerAction* ServerActionMotd::clone() const {
    return (new ServerActionMotd(*this));
}

