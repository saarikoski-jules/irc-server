/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   welcome_client.cpp                                :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/07 14:24:01 by jules        #+#    #+#                  */
/*   Updated: 2021/05/07 15:14:48 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "welcome_client.h"

#include <ctime>

#include "reply.h"
#include "server.h"
#include "action_factory.h"
#include "reply.h"

void welcomeClient(Server* server, const int& fd, const std::string& prefix) {
	actionFactory factory;
	std::vector<std::string> params;
    std::string reply;

    Connection* client = server->getConnectionByFd(fd);
    std::string clientName(client->client.nickName + "!" + client->client.userName + "@" + client->client.hostName);
    params.push_back(client->client.nickName);
    params.push_back(clientName);
    reply = ReplyFactory::newReply(RPL_WELCOME, params);
    server->sendReplyToClient(fd, reply);

    params.pop_back();
    reply = ReplyFactory::newReply(RPL_YOURHOST, params);
    server->sendReplyToClient(fd, reply);

    struct tm * timeinfo;

    timeinfo = localtime ( &(server->serverStart) );
    char* timechars = asctime(timeinfo);
    std::string timestr(timechars);
    timestr.pop_back();

    params.push_back(timestr);
    reply = ReplyFactory::newReply(RPL_CREATED, params);
    server->sendReplyToClient(fd, reply);

    params.pop_back();
    reply = ReplyFactory::newReply(RPL_MYINFO, params);
    server->sendReplyToClient(fd, reply);

    IServerAction* action = factory.newAction("MOTD", params, fd, prefix);
	server->addNewAction(action);
}

