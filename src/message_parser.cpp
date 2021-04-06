#include "message_parser.h"

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "utils.h"
#include "action_factory.h"

#include "logger.h"

void MessageParser::parse(const std::string& data, const int& clientFd) {
    std::vector<std::string> commands = Utils::String::tokenize(data, "\n");
    if (*commands.end() == "") {
        commands.pop_back();
    }

    IServerAction* action;
    actionFactory factory;
    while (commands.size() != 0) {
        std::vector<std::string>cmd = Utils::String::tokenize(commands[0], " ");
        std::string cmdType = cmd.front();
        cmd.erase(cmd.begin());
        try {
            action = factory.newAction(cmdType, cmd, clientFd);
        } catch (const ActionFactoryException& e) {
            if (e.isFatal() == false) {
                Logger::log(LogLevelDebug, "Invalid new action");
            }
        }
        commands.erase(commands.begin());
    }
}
