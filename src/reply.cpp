/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   reply.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/09 15:41:18 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/09 17:43:45 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "reply.h"

#include <string>
#include <vector>
#include <sstream>

std::string ReplyFactory::newReply(const ReplyCode& code, std::vector<std::string> params) {
    (void)params;
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
    default:
        break;
    }
    return (ss.str());
}
