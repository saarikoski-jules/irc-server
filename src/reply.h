/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 14:49:32 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/21 16:27:30 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_H_
#define REPLY_H_

#include <string>
#include <vector>

enum ReplyCode {
    ERR_NONICKNAMEGIVEN = 431,
    ERR_NICKNAMEINUSE = 433,
    ERR_BADCHANNELKEY = 475,
    ERR_NOSUCHCHANNEL = 403,
    ERR_NEEDMOREPARAMS = 461,
    ERR_NOSUCHNICK = 401,
    ERR_CHANOPRIVSNEEDED = 482,
    ERR_UNKNOWNMODE = 472,
    ERR_ALREADYREGISTERED = 462,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_CHANNELMODEIS = 324,
    RPL_BANLIST = 367,
    RPL_ENDOFBANLIST = 368
};

class ReplyFactory {
 public:
    static std::string newReply(const ReplyCode& code, std::vector<std::string> params);
};

#endif  // REPLY_H_
