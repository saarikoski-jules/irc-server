/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   construct_reply.h                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:53:28 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 11:58:16 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSTRUCT_REPLY_H_
#define CONSTRUCT_REPLY_H_

#include <string>

std::string constructNoSuchChannelReply(const std::string& cliNick, const std::string& chanName);
std::string constructNoSuchNickReply(const std::string& cliNick, const std::string& targetNick);
std::string constructChanoPrivsNeededReply(const std::string& cliNick, const std::string& chanName);

#endif  // CONSTRUCT_REPLY_H_