/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 13:30:35 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/08 15:50:38 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_parser.h"

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

#include "utils.h"
#include "action_factory.h"
#include "iserver_action.h"
#include "logger.h"

struct MessageParser::isNotAlpha {
    bool operator()(char c) {
        return !std::isalpha(c);
    }
};

struct MessageParser::isNotDigit {
    bool operator()(char c) {
        return !std::isdigit(c);
    }
};

void MessageParser::validCommand(std::string cmd) const {
    if (std::find_if(cmd.begin(), cmd.end(), isNotAlpha()) != cmd.end()) {
        if (std::find_if(cmd.begin(), cmd.end(), isNotDigit()) != cmd.end() && cmd.length() != 3) {
            throw MessageParserException("Invalid command", false);
        }
    }
}

std::vector<std::string> MessageParser::validParams(std::vector<std::string>::iterator i, std::vector<std::string>::iterator end) const {
    std::vector<std::string> params;

    for (; i != end; i++) {
        if ((*i)[0] == ':') {
            if (i + 1 != end) {
                throw MessageParserException("Invalid parameters", false);
            }
            (*i).erase(0, 1);
        }
        if ((*i) != "") {
            // TODO(Jules): doesnt contain SPACE or NUL or CR or LF,
            params.push_back(*i);
        }
    }
    return (params);
}


IServerAction* MessageParser::createActionFromMessage(const std::string& message, const int& clientFd) {
    IServerAction* action;
    std::string cmd;
    std::vector<std::string> params;

    actionFactory factory;

    std::vector<std::string> splitMsg = Utils::String::tokenize(message, " ");
    std::vector<std::string>::iterator i = splitMsg.begin();

    if (splitMsg[0][0] == ':') {
        i++;
    }
    while ((*i) == "") {
        i++;
    }
    validCommand(*i);
    cmd = *i;
    i++;
    params = validParams(i, splitMsg.end());

    try {
        action = factory.newAction(cmd, params, clientFd);
    } catch (const ActionFactoryException& e) {
        throw MessageParserException(e.what(), e.isFatal());
    }
    Logger::log(LogLevelDebug, "created new action");
    return (action);
}

std::vector<IServerAction*> MessageParser::parse(const std::string& data, const int& clientFd) {
    std::vector<IServerAction*> actions;
    std::vector<std::string> commands = Utils::String::tokenize(data, "\r\n");

    for(std::vector<std::string>::iterator i = commands.begin(); i != commands.end(); i++) {
        try {
            IServerAction* action = createActionFromMessage(*i, clientFd);
            actions.push_back(action);
        } catch (const MessageParserException& e) {
            Logger::log(LogLevelDebug, "Couldn't generate action from message");
        }
    }
    return (actions);
}

MessageParserException::MessageParserException(std::string const& what, const bool& fatal) :
fatal(fatal),
message(what) {
}

MessageParserException::~MessageParserException() throw() {
}

const bool& MessageParserException::isFatal() const {
    return (fatal);
}

const char* MessageParserException::what() const throw() {
    if (isFatal()) {
        return (std::string("Fatal message parser exception: " + message).c_str());
    }
    return (std::string("Message parser exception: " + message).c_str());
}
