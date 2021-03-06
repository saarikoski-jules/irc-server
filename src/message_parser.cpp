/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.cpp                                :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 13:30:35 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/09 15:06:46 by jules        ########   odam.nl          */
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
#include "server_action_store.h"

#define LINEBREAK "\r\n"

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
    if (cmd.length() == 0) {
        throw MessageParserException("Invalid command", false);
    }
    if (std::find_if(cmd.begin(), cmd.end(), isNotAlpha()) != cmd.end()) {
        if (std::find_if(cmd.begin(), cmd.end(), isNotDigit()) != cmd.end() && cmd.length() != 3) {
            throw MessageParserException("Invalid command", false);
        }
    }
}

std::string MessageParser::genPrefix(std::string* message, std::string::iterator* it) const {
    std::string::iterator end = message->begin();

    if (*end == ':') {
        end = std::find(message->begin(), message->end(), ' ');
        std::string prefix(message->begin() + 1, end);
        *it = end;
        (*it)++;
        return (prefix);
    }
    return ("");
}

std::string MessageParser::genCommand(std::string* message, std::string::iterator* it) const {
    std::string::iterator end;

    end = std::find(*it, message->end(), ' ');
    std::string cmd(*it, end);
    validCommand(cmd);
    *it = end;
    (*it)++;
    return (cmd);
}

std::vector<std::string> MessageParser::genParams(
    std::string* message, std::string::iterator* it) const {
    std::vector<std::string> params;
    if (std::distance(*it, message->end()) <= 0) {
        return (params);
    }
    std::string str(*it, message->end());
    std::string::size_type len;
    if (str[0] == ':') {
        len = 0;
    } else {
        len = str.find(" :");
        params = Utils::String::tokenize(str, len, " ");
    }

    if (len != std::string::npos) {
        std::string final;
        if (len == 0) {
            final = std::string(str, len + 1);
        } else {
            final = std::string(str, len + 2);
        }
        params.push_back(final);
    }
    *it = message->end();
    return (params);
}

IServerAction* MessageParser::createActionFromMessage(std::string message, const int& fd) {
    std::string prefix;
    std::string cmd;
    std::vector<std::string> params;

    actionFactory factory;
    std::string::iterator it = message.begin();

    IServerAction* action;
    try {
        prefix = genPrefix(&message, &it);
        cmd = genCommand(&message, &it);
        params = genParams(&message, &it);
        action = factory.newAction(cmd, params, fd, prefix);
    } catch (const ActionFactoryException& e) {
        throw MessageParserException(e.what(), e.isFatal());
    } catch (const std::exception& e) {
        throw MessageParserException(e.what(), false);
    }
    Logger::log(LogLevelDebug, "created new action");
    return (action);
}

std::vector<IServerAction*> MessageParser::parse(const std::string& data, const int& fd) {
    std::vector<IServerAction*> actions;
	std::vector<std::string> commands = Utils::String::tokenize(data, data.length(), LINEBREAK);

	bool isPartial;
	try {
		if (data.substr(data.length() - 2) == LINEBREAK) {
			isPartial = false;
		} else {
			isPartial = true;
		}
	} catch (const std::exception& e) {
		isPartial = true;
	}
	

    for (std::vector<std::string>::iterator i = commands.begin(); i != commands.end(); i++) {
        try {
			if (isPartial && i + 1 == commands.end()) {
				std::vector<std::string> params(1, *i);
				IServerAction* action = new ServerActionStore(params, fd, ""); 
				actions.push_back(action);
			} else {
				IServerAction* action = createActionFromMessage(*i, fd);
				actions.push_back(action);
			}
		} catch (const MessageParserException& e) {
            Logger::log(LogLevelDebug, "Couldn't generate action from message");
        }
    }
    return (actions);
}

MessageParserException::MessageParserException(std::string const& what, const bool& fatal) :
fatal(fatal),
message(what) {
    if (isFatal()) {
        fullMessage = std::string("Fatal message parser exception: " + message);
    } else {
        fullMessage = std::string("Message parser exception: " + message);
    }
}

MessageParserException::~MessageParserException() throw() {
}

const bool& MessageParserException::isFatal() const {
    return (fatal);
}

const char* MessageParserException::what() const throw() {
    return (fullMessage.c_str());
}
