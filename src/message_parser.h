/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:18 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 17:00:48 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEPARSER_H_
#define MESSAGEPARSER_H_

#include <string>

#include "client.h"

class MessageParser {
 public:
    void parse(const std::string& data, const int& clientFd);
};

#endif  //  MESSAGEPARSER_H_
