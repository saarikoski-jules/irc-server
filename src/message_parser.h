/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:18 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/08 13:24:11 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_PARSER_H_
#define MESSAGE_PARSER_H_

#include <string>
#include <vector>

#include "client.h"
#include "iserver_action.h"

class MessageParser {
 public:
    std::vector<IServerAction*> parse(const std::string& data, const int& clientFd);
 private:
    std::vector<std::string> validParams(std::vector<std::string>::iterator i, std::vector<std::string>::iterator end) const;
    IServerAction* createActionFromMessage(const std::string& command, const int& clientFd);
    void validCommand(const std::string& cmd) const;
};

class MessageParserException: public std::exception {
 public:
    MessageParserException(std::string const& what, const bool& fatal);
    ~MessageParserException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
};

#endif  // MESSAGE_PARSER_H_
