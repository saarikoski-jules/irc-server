#include "message_parser.h"

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "utils.h"
#include "server_action.h"

#include "logger.h"

void MessageParser::parse(const std::string& data, const std::vector<Client>& clients) {
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
        Logger::log(LogLevelInfo, "cmd: " + cmdType);
        Logger::log(LogLevelInfo, "first param: " + cmd.front());
        try {
            action = factory.newAction(cmdType, cmd, clients);
        } catch (const ActionFactoryException& e) {
            if (e.isFatal() == false) {
                Logger::log(LogLevelDebug, "Invalid new action");
            }
        }
        commands.erase(commands.begin());
        //send numeric reply when needed
    }
}
