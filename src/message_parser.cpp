/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   message_parser.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/07 12:16:44 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/07 12:16:46 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "message_parser.h"

#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "utils.h"
#include "action_factory.h"
#include "iserver_action.h"

#include "logger.h"

std::vector<IServerAction*> MessageParser::parse(const std::string& data, const int& clientFd) {
    std::vector<IServerAction*> actions;
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
            actions.push_back(action);
            Logger::log(LogLevelDebug, "created new action");
        } catch (const ActionFactoryException& e) {
            if (e.isFatal() == false) {
                Logger::log(LogLevelDebug, "Invalid new action");
            }
        }
        commands.erase(commands.begin());
    }
    return (actions);
}
