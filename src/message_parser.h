/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:18 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 18:02:00 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_PARSER_H_
#define MESSAGE_PARSER_H_

#include <string>

#include "client.h"

class MessageParser {
 public:
    void parse(const std::string& data, const int& clientFd);
};

#endif  // MESSAGE_PARSER_H_
