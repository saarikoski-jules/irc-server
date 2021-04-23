/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 15:41:18 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/21 16:28:25 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "reply.h"

#include <string>
#include <vector>
#include <sstream>

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
    case ERR_BADCHANNELKEY:
        ss << params[1] << " :Cannot join channel (+k)";
        break;
    case ERR_NOSUCHCHANNEL:
        ss << params[1] << " :No such channel";
        break;
    case ERR_NEEDMOREPARAMS:
        ss << params[1] << " :Not enough parameters";
        break;
    case ERR_NOSUCHNICK:
        ss << params[1] << " :No such nick/channel";
        break;
    case ERR_CHANOPRIVSNEEDED:
        ss << params[1] << " :You're not channel operator";
        break;
    case ERR_UNKNOWNMODE:
        ss << params[1] << " :is unknown mode char to me";
	break;
    case ERR_ALREADYREGISTERED:
        ss << " :You may not reregister";
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
    default:
        break;
    }
    return (ss.str());
}
