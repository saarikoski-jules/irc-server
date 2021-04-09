/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 13:30:35 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/09 18:01:50 by jsaariko      ########   odam.nl         */
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

//TODO(Jules): store prefix for checking
std::string MessageParser::genCommand(std::string& message, std::string::iterator* it) const {
    std::string::iterator start = message.begin();
    std::string::iterator end;

    if (*start == ':') {
        start = std::find(message.begin(), message.end(), ' ');
        start++;
    }
    end = std::find(start, message.end(), ' ');
    std::string cmd(start, end);
    validCommand(cmd);

    *it = end;
    (*it)++;
    return (cmd);
}

std::vector<std::string> MessageParser::genParams(std::string& message, std::string::iterator* it) const {
    std::vector<std::string> params;
    std::string str(*it, message.end());
    std::string::size_type len = str.find(" :");

    params = Utils::String::tokenize(str, len, " ");

    std::string final(str, len + 2);
    params.push_back(final);
    *it = message.end();
    return (params);
}

IServerAction* MessageParser::createActionFromMessage(std::string message, const int& clientFd) {
    std::string cmd;
    std::vector<std::string> params;

    actionFactory factory;
    std::string::iterator it = message.begin();

    IServerAction* action;

    cmd = genCommand(message, &it);
    params = genParams(message, &it);

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
    #ifndef DEBUG
        std::vector<std::string> commands = Utils::String::tokenize(data, data.length(), "\r\n");
    #elif
        std::vector<std::string> commands = Utils::String::tokenize(data, data.length(), "\n");
    #endif
    // TODO(Jules): remove from final version

    for (std::vector<std::string>::iterator i = commands.begin(); i != commands.end(); i++) {
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
