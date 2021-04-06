/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:18 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 14:08:46 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEPARSER_H_
#define MESSAGEPARSER_H_

#include <string>

#include "client.h"

class MessageParser {
 public:
    void parse(const std::string& data, const std::vector<Client>& clients);
};

#endif  //  MESSAGEPARSER_H_
