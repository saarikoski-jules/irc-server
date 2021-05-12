/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.cpp                                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 15:41:18 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/12 11:14:41 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "reply.h"

#include <string>
#include <vector>
#include <sstream>  // TODO(Jules): Bad

#include "server.h"

std::string ReplyFactory::newReply(const ReplyCode& code, std::vector<std::string> params) {
    std::stringstream ss;

    ss << code << " " << params[0] << " ";
    switch (code)
    {
    case ERR_NONICKNAMEGIVEN:
        ss << ":No nickname given";
        break;
    case ERR_NICKNAMEINUSE:
        ss << params[1] << " :Nickname is already in use";
        break;
    case ERR_ERRONEUSNICKNAME:
        ss << params[1] << " :Erroneus nickname";
        break;
    case ERR_BADCHANNELKEY:
        ss << params[1] << " :Cannot join channel (+k)";
        break;
    case ERR_NOSUCHCHANNEL:
        ss << params[1] << " :No such channel";
        break;
    case ERR_NOTREGISTERED:
        ss << " :You have not registered";
        break;
    case ERR_NEEDMOREPARAMS:
        ss << params[1] << " :Not enough parameters";
        break;
    case ERR_NOSUCHNICK:
        ss << params[1] << " :No such nick/channel";
        break;
    case ERR_NOPRIVILEGES:
        ss << " :Permission Denied- You're not an IRC operator";
    	break;
	case ERR_CHANOPRIVSNEEDED:
        ss << params[1] << " :You're not channel operator";
        break;
    case ERR_UNKNOWNMODE:
        ss << params[1] << " :is unknown mode char to me";
        break;
    case ERR_NOTEXTTOSEND:
        ss << ":No text to send";
        break;
    case ERR_NORECIPIENT:
        ss << ":No recipient given " << params[1];
        break;
    case ERR_ALREADYREGISTERED:
        ss << " :You may not reregister";
        break;
    case ERR_CANNOTSENDTOCHAN:
        ss << params[1] << " :Cannot send to chan";
        break;
    case RPL_TOPIC:
        ss << params[1] << " :" << params[2];
        break;
    case RPL_NOTOPIC:
        ss << params[1] << " :No topic is set";
        break;
    case RPL_CHANNELMODEIS:
        ss << params[1] << " " << params[2] << " " << params[3];
        break;
    case RPL_BANLIST:
        ss << params[1] << " " << params[2];
        break;
    case RPL_ENDOFBANLIST:
        ss << params[1] << " :End of channel ban list";
        break;
	case RPL_MOTDSTART:
		ss << ":- " << SERVERNAME << " Message of the day - ";
		break;
	case RPL_MOTD:
		ss << ":- " << params[1];
		break;
	case RPL_ENDOFMOTD:
		ss << ": End of /MOTD command";
		break;
	case RPL_WELCOME:
		ss << ":Welcome to the Internet Relay Network " << params[1];
		break;
	case RPL_YOURHOST:
		ss << ":Your host is " << SERVERNAME << ", running version 2.11";
		break;
	case RPL_CREATED:
		ss << ":This server was created " << params[1];
		break;
	case RPL_MYINFO:
		ss << SERVERNAME << " 2.11  pstnolbk";
   		break;
	case RPL_NAMREPLY:
        ss << params[1] << " :" << params[2];
        break;
    case RPL_ENDOFNAMES:
        ss << params[1] << " :End of /NAMES list";
        break;
    default:
        break;
    }
    return (ss.str());
}
