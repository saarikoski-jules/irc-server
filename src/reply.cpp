/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.cpp                                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 15:41:18 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/02 15:52:46 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "reply.h"

#include <string>
#include <vector>

#include "server.h"
#include "int_conversions.h"

std::string ReplyFactory::newReply(const ReplyCode& code, std::vector<std::string> params) {

    std::string init(IntConversion::intToString(code) + " " + params[0] + " ");
    std::string res;
    switch (code)
    {
    case ERR_NONICKNAMEGIVEN:
        res = std::string(init + ":No nickname given");
        break;
    case ERR_NICKNAMEINUSE:
        res = std::string(init + params[1] + " :Nickname is already in use");
        break;
    case ERR_ERRONEUSNICKNAME:
        res = std::string(init + params[1] + " :Erroneus nickname");
        break;
    case ERR_BADCHANNELKEY:
        res = std::string(init + params[1] + " :Cannot join channel (+" + params[2] + ")");
        break;
    case ERR_NOSUCHCHANNEL:
        res = std::string(init + params[1] + " :No such channel");
        break;
    case ERR_NOTREGISTERED:
        res = std::string(init + " :You have not registered");
        break;
    case ERR_NEEDMOREPARAMS:
        res = std::string(init + params[1] + " :Not enough parameters");
        break;
    case ERR_PASSWDMISMATCH:
        res = std::string(init + " :Password incorrect");
        break;
    case ERR_NOSUCHNICK:
        res = std::string(init + params[1] + " :No such nick/channel");
        break;
    case ERR_NOPRIVILEGES:
        res = std::string(init + " :Permission Denied- You're not an IRC operator");
    	break;
    case ERR_CHANOPRIVSNEEDED:
        res = std::string(init + params[1] + " :You're not channel operator");
        break;
    case ERR_UNKNOWNMODE:
        res = std::string(init + params[1] + " :is unknown mode char to me");
        break;
    case ERR_NOTEXTTOSEND:
        res = std::string(init + ":No text to send");
        break;
    case ERR_NORECIPIENT:
        res = std::string(init + ":No recipient given " + params[1]);
        break;
    case ERR_ALREADYREGISTERED:
        res = std::string(init + " :You may not reregister");
        break;
    case ERR_CANNOTSENDTOCHAN:
        res = std::string(init + params[1] + " :Cannot send to chan");
        break;
	case ERR_NOTONCHANNEL:
		res = std::string(init + params[1] + ":You're not on that channel");
		break;
	case ERR_UMODEUNKNOWNFLAG:
		res = std::string(init + ":Unknown MODE flag");
		break;
	case ERR_USERSDONTMATCH:
		res = std::string(init + ":Can't change mode for other users");
    	break;
	case ERR_NOSUCHSERVER:
		res = std::string(init + params[1] + " :No such server");
		break;
	case RPL_TOPIC:
        res = std::string(init + params[1] +  " :" + params[2]);
        break;
    case RPL_NOTOPIC:
        res = std::string(init + params[1] + " :No topic is set");
        break;
    case RPL_CHANNELMODEIS:
        res = std::string(init + params[1]);
        break;
    case RPL_BANLIST:
        res = std::string(init + params[1] + " " + params[2]);
        break;
    case RPL_ENDOFBANLIST:
        res = std::string(init + params[1] + " :End of channel ban list");
        break;
	case RPL_MOTDSTART:
		res = std::string(init +  ":- " + Server::serverName + " Message of the day - ");
		break;
	case RPL_MOTD:
		res = std::string(init +  ":- " + params[1]);
		break;
	case RPL_ENDOFMOTD:
		res = std::string(init +  ": End of /MOTD command");
		break;
	case RPL_WELCOME:
		res = std::string(init +  ":Welcome to the Internet Relay Network " + params[1]);
		break;
	case RPL_YOURHOST:
		res = std::string(init +  ":Your host is " + Server::serverName + ", running version 2.11");
		break;
	case RPL_CREATED:
		res = std::string(init +  ":This server was created " + params[1]);
		break;
	case RPL_MYINFO:
		res = std::string(init +  Server::serverName + " 2.11  pstnolbk");
   		break;
    case RPL_SERVERINFO:
		res = std::string(init +  "CHANTYPES=#& CHANMODE=pstnolbk MSGLEN=512 CASEMAPPING=ascii");
   		break;
	case RPL_NAMREPLY:
        res = std::string(init +  params[1] + " " + params[2] + " :" + params[3]);
        break;
    case RPL_ENDOFNAMES:
        res = std::string(init +  params[1] + " :End of /NAMES list");
        break;
	case RPL_UMODEIS:
		res = std::string(init + params[1]);
		break;
	case RPL_WHOREPLY:
		res = std::string(init + params[1] + " " + params[2] + " " + params[3] + " " + params[4] + " " + params[5] + " " + params[6] + " :" + params[7] + " " + params[8]);
		break;
	case RPL_ENDOFWHO:
		res = std::string(init + params[1] + " :End of /WHO list");
		break;
	default:
        res = init;
        break;
    }
    return (res);
}
