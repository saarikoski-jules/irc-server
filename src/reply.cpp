/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 15:41:18 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/14 12:33:59 by jsaariko      ########   odam.nl         */
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
        ss << ":Cannot join channel (+k)";
        break;
    case ERR_NOSUCHCHANNEL:
        ss << ":No such channel";
        break;
    case ERR_NEEDMOREPARAMS:
        ss << ":Not enough parameters";
        break;
    case RPL_TOPIC:
        ss << ":" << params[1];
        break;
    case RPL_NOTOPIC:
        ss << ":No topic is set";
        break;
    default:
        break;
    }
    return (ss.str());
}
