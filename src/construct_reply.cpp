/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   construct_reply.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:53:00 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 12:02:44 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "construct_reply.h"

#include "reply.h"

std::string constructNoSuchChannelReply(const std::string& cliNick, const std::string& chanName) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(chanName);
    reply = ReplyFactory::newReply(ERR_NOSUCHCHANNEL, replyParams);
    return (reply);
}

std::string constructNoSuchNickReply(const std::string& cliNick, const std::string& targetNick) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(targetNick);
    reply = ReplyFactory::newReply(ERR_NOSUCHNICK, replyParams);
    return (reply);
}

std::string constructChanoPrivsNeededReply(const std::string& cliNick, const std::string& chanName) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(chanName);
    reply = ReplyFactory::newReply(ERR_CHANOPRIVSNEEDED, replyParams);
    return (reply);
}
