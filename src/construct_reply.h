/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   construct_reply.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:53:28 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/21 16:29:13 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTRUCT_REPLY_H_
#define CONSTRUCT_REPLY_H_

#include <vector>
#include <string>

#include "connection.h"

std::string constructNoSuchChannelReply(const std::string& cliNick, const std::string& chanName);
std::string constructNoSuchNickReply(const std::string& cliNick, const std::string& targetNick);
std::string constructChanoPrivsNeededReply(const std::string& cliNick, const std::string& chanName);
std::string constructNeedMoreParamsReply(const std::string& cliNick, const std::string& command);
std::string constructNoTextToSendReply(const std::string& cliNick);
std::string constructNoRecipientReply(const std::string& cliNick, const std::string& command);
std::string constructCannotSendToChanReply(const std::string& senderNick, const std::string& chanName);
std::string constructNotRegisteredReply(const std::string& name);
std::string constructNewNickBroadcast(const Connection& connection);
std::string constructNickChangeBroadcast(const std::string& oldNickName, const std::string& newNickName);
std::string constructServerNickBroadcast(const std::vector<std::string> params);
std::string constructNewServerBroadcast(const Connection& connection);
std::string constructServerValidation();
std::string constructJoinBroadcast(const std::string& nickName, const std::string& channelName);
std::string constructOperModeBroadcast(const std::string& nickName, const std::string& channelName);
std::string constructNoOperModeBroadcast(const std::string& nickName, const std::string& channelName);

#endif  // CONSTRUCT_REPLY_H_
