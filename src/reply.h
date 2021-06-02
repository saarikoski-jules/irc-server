/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 14:49:32 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/02 12:12:21 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_H_
#define REPLY_H_

#include <string>
#include <vector>

enum ReplyCode {
    ERR_NONICKNAMEGIVEN = 431,
    ERR_NICKNAMEINUSE = 433,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_BADCHANNELKEY = 475,
    ERR_NOSUCHCHANNEL = 403,
    ERR_NOTREGISTERED = 451,
    ERR_NEEDMOREPARAMS = 461,
    ERR_NOSUCHNICK = 401,
    ERR_NOPRIVILEGES = 481,
    ERR_CHANOPRIVSNEEDED = 482,
    ERR_UNKNOWNMODE = 472,
    ERR_ALREADYREGISTERED = 462,
    ERR_NOTEXTTOSEND = 412,
    ERR_NORECIPIENT = 411,
    ERR_CANNOTSENDTOCHAN = 404,
	ERR_NOMOTD = 422,
	ERR_NOTONCHANNEL = 442,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502,
	ERR_NOSUCHSERVER = 402,
	RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_CHANNELMODEIS = 324,
    RPL_BANLIST = 367,
    RPL_ENDOFBANLIST = 368,
	RPL_MOTDSTART = 375,
	RPL_MOTD = 372,
	RPL_ENDOFMOTD = 376,
	RPL_WELCOME = 001,
	RPL_YOURHOST = 002,
	RPL_CREATED = 003,
	RPL_MYINFO = 004,
    RPL_SERVERINFO = 005,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
	RPL_UMODEIS = 221,
	RPL_WHOREPLY = 352,
	RPL_ENDOFWHO = 315
};

class ReplyFactory {
 public:
    static std::string newReply(const ReplyCode& code, std::vector<std::string> params);
};

#endif  // REPLY_H_
