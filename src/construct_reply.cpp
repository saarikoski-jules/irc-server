/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   construct_reply.cpp                               :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:53:00 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/26 13:01:18 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "construct_reply.h"

#include <vector>

#include "reply.h"
#include "connection.h"
#include "server.h"

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

std::string constructChanoPrivsNeededReply(
    const std::string& cliNick, const std::string& chanName) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(chanName);
    reply = ReplyFactory::newReply(ERR_CHANOPRIVSNEEDED, replyParams);
    return (reply);
}

std::string constructNeedMoreParamsReply(
	const std::string& cliNick, const std::string& command) {
	std::string reply;
	std::vector<std::string> replyParams;

	replyParams.push_back(cliNick);
	replyParams.push_back(command);
	reply = ReplyFactory::newReply(ERR_NEEDMOREPARAMS, replyParams);
	return (reply);
}

std::string constructNoTextToSendReply(
    const std::string& cliNick) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    reply = ReplyFactory::newReply(ERR_NOTEXTTOSEND, replyParams);
    return (reply);
}

std::string constructNoRecipientReply(
    const std::string& cliNick, const std::string& command) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(cliNick);
    replyParams.push_back(command);
    reply = ReplyFactory::newReply(ERR_NORECIPIENT, replyParams);
    return (reply);
}

std::string constructCannotSendToChanReply(
    const std::string& senderNick, const std::string& chanName) {
    std::string reply;
    std::vector<std::string> replyParams;

    replyParams.push_back(senderNick);
    replyParams.push_back(chanName);
    reply = ReplyFactory::newReply(ERR_CANNOTSENDTOCHAN, replyParams);
    return (reply);
}

std::string constructNotRegisteredReply(const std::string& name) {
    std::string reply;
    std::vector<std::string> params;

    params.push_back(name);
    reply = ReplyFactory::newReply(ERR_NOTREGISTERED, params);
    return (reply);
}

std::string constructNewNickBroadcast(const Connection& connection) {
    const Client& client = connection.client;
    std::string reply(":" + Server::serverName + " "
        "NICK "
        + client.nickName
        + " 1 "
        + client.userName + " "
        + client.hostName + " "
        SERVERTOKEN
        " + "
        ":" + client.realName + "\r\n");
    return (reply);
}

std::string constructNickChangeBroadcast(const std::string& oldNickName, const std::string& newNickName) {
    std::string reply(":" + oldNickName + " NICK :" + newNickName + "\r\n");
    return (reply);
}

std::string constructServerNickBroadcast(const std::vector<std::string> params) {
    std::string reply(":" + Server::serverName + " "
        "NICK "
        + params[0]
        + " 1 "
        + params[2] + " "
        + params[3] + " "
        + params[4] + " "
        + params[5] + " "
        + ":" + params[6] + "\r\n");
    return (reply);
}

std::string constructNewServerBroadcast(const Connection& connection) {
    std::string reply(":" + Server::serverName + " "
        "SERVER "
        + connection.server.name + " "
        + "1 "
        + connection.server.token + " "
        + ":" + connection.server.info + "\r\n");
    return (reply);
}

std::string constructServerValidation() {
    std::string reply(
        "PASS "
        SERVER_CONNECTION_PASSWORD " "
        "0211 "
        "IRC|\r\n"

        "SERVER "
        + Server::serverName + " "
        "1 "
        SERVERTOKEN " "
        ":Codam development irc\r\n");
    return (reply);
}

std::string constructNotOnChannelReply(const std::string& nickName, const std::string& chanName) {
	std::string reply;
	std::vector<std::string> params;
	params.push_back(nickName);
	params.push_back(chanName);
	reply = ReplyFactory::newReply(ERR_NOTONCHANNEL, params);
	return (reply);
}

std::string constructJoinBroadcast(const std::string& nickName, const std::string& channelName) {
    std::string reply(
        ":" + nickName + " "
        "JOIN "
        ":" + channelName + "\r\n");
    return (reply);
}

std::string constructOperModeBroadcast(const std::string& nickName, const std::string& channelName) {
    std::string reply(
        ":" + Server::serverName + " "
        "MODE "
        + channelName + " "
        "+o "
        ":" + nickName + "\r\n");
    return (reply);
}

std::string constructNoOperModeBroadcast(const std::string& nickName, const std::string& channelName) {
    std::string reply(
        ":" + Server::serverName + " "
        "MODE "
        + channelName + " "
        "-o "
        ":" + nickName + "\r\n");
    return (reply);
}
