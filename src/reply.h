/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 14:49:32 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/13 18:34:06 by jsaariko      ########   odam.nl         */
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
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332
};

class ReplyFactory {
 public:
    static std::string newReply(const ReplyCode& code, std::vector<std::string> params);
};

#endif  // REPLY_H_
