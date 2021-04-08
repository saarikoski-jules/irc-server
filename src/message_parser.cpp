#include "message_parser.h"

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "utils.h"
#include "action_factory.h"
#include "iserver_action.h"

#include "logger.h"


#include <iostream>
#include <algorithm>

struct isNotAlpha {
    bool operator()(char c) {
        return !std::isalpha(c);
    }
};

struct isNotDigit {
    bool operator()(char c) {
        return !std::isdigit(c);
    }
};

void MessageParser::validCommand(const std::string& cmd) const {
    if (std::find_if(cmd.begin(), cmd.end(), isNotAlpha()) != cmd.end()) {
        if (std::find_if(cmd.begin(), cmd.end(), isNotDigit()) != cmd.end() && cmd.length() != 3) {
            throw MessageParserException("Invalid command", false);
        }
    }
}

std::vector<std::string> MessageParser::validParams(std::vector<std::string>::iterator i, std::vector<std::string>::iterator end) const {
    std::vector<std::string> params;

    for (; i != end; i++) {
        Logger::log(LogLevelDebug, std::string("current param checking: " + *i));
        if ((*i)[0] == ':') {
            if (i + 1 != end) {
                Logger::log(LogLevelDebug, "failing with last param: " + *(i + 1));
                throw MessageParserException("Invalid parameters", false);
            }
        } else if ((*i) != "") {
            //TODO(Jules): doesnt contain SPACE or NUL or CR or LF,
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
    action = factory.newAction(cmd, params, clientFd);
    Logger::log(LogLevelDebug, "created new action");
    return (action);
}

std::vector<IServerAction*> MessageParser::parse(const std::string& data, const int& clientFd) {
    std::vector<IServerAction*> actions;
    std::vector<std::string> commands = Utils::String::tokenize(data, "\r\n");

    while (commands.size() != 0) {
        try {
            IServerAction* action = createActionFromMessage(commands[0], clientFd);
            actions.push_back(action);
        } catch (const MessageParserException& e) {
            Logger::log(LogLevelDebug, "Couldn't generate action from message");
        }
        commands.erase(commands.begin());
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
