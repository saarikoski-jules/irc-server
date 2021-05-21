/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_motd.cpp                            :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/07 12:05:23 by jules         #+#    #+#                 */
/*   Updated: 2021/05/20 15:53:58 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_motd.h"

#include "server.h"
#include "construct_reply.h"
#include "utils.h"

ServerActionMotd::ServerActionMotd(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

static std::string const motd[] = {
"===========================================================",
" ",
"This server is provided by the blood, sweat and tears of",
"jvisser and jsaariko",
" ",
"Server administrator: ANARCHY",
" ",
" ",
"          @@@    @@@@@@   @@@  @@@@@@@    @@@@@@@  ",
"         @@@@   @@@@@@@@  @@@  @@@@@@@@  @@@@@@@@  ",
"        @@!@!        @@@  @@!  @@!  @@@  !@@       ",
"       !@!!@!       @!@   !@!  !@!  @!@  !@!       ",
"      @!! @!!      !!@    !!@  @!@!!@!   !@!       ",
"     !!!  !@!     !!:     !!!  !!@!@!    !!!       ",
"     :!!:!:!!:   !:!      !!:  !!: :!!   :!!       ",
"     !:::!!:::  :!:       :!:  :!:  !:!  :!:       ",
"          :::   :: :::::   ::  ::   :::   ::: :::  ",
"          :::   :: : :::  :     :   : :   :: :: :  ",
" ",
"===========================================================",
" ",
"Welcome to Incredulously Reputable Chat. The chat that",
"definitely doesn't have a hardcoded password as a define",
"in the code.",
" ",
"To prevent abuse we do some error checking, but you can",
"probably steal a nick if you try hard enough.",
" ",
"----------------------------------------------------------",
"          IRC is a privilege, not a right!",
"    Respect others and enjoy your stay on IRCnet.",
"        ..or don't, we are ruled by anarchy!",
"----------------------------------------------------------"
};

void ServerActionMotd::execute() {
   Connection* connection = server->getConnectionByFd(fd);
   switch(connection->connectionType) {
        case Connection::ServerType:
   			return;
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

    params.push_back("aa");
    for (size_t i = 0; i < 33; i++) {
        params.pop_back();
        params.push_back(motd[i]);
        reply = ReplyFactory::newReply(RPL_MOTD, params);
        server->sendReplyToClient(fd, reply);
    }
    server->sendReplyToClient(fd, ReplyFactory::newReply(RPL_ENDOFMOTD, params));
}

IServerAction* ServerActionMotd::clone() const {
    return (new ServerActionMotd(*this));
}

