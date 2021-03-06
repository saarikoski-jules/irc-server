/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:18 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/23 11:17:06 by jvisser       ########   odam.nl         */
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
    std::vector<IServerAction*> parse(const std::string& data, const int& fd);
 private:
    IServerAction* createActionFromMessage(std::string command, const int& fd);
    std::string genPrefix(std::string* message, std::string::iterator* it) const;
    std::string genCommand(std::string* message, std::string::iterator* it) const;
    std::vector<std::string> genParams(std::string* message, std::string::iterator* it) const;
    void validCommand(std::string cmd) const;
    struct isNotAlpha;
    struct isNotDigit;
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
    std::string fullMessage;
};

#endif  // MESSAGE_PARSER_H_
