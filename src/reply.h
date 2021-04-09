/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 14:49:32 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/09 17:43:08 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_H_
#define REPLY_H_

#include <string>
#include <vector>

enum ReplyCode {
    ERR_NONICKNAMEGIVEN = 431,
    ERR_NICKNAMEINUSE = 433
};

class ReplyFactory {
 public:
    static std::string newReply(const ReplyCode& code, std::vector<std::string> params);
};

#endif  // REPLY_H_
