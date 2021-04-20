/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_receive.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:38:56 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 11:41:10 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_receive.h"

#include <vector>

#include "message_parser.h"
#include "server.h"

ServerActionReceive::ServerActionReceive(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 1, cli, prefix),
params(params) {}

void ServerActionReceive::execute() {
    // Logger::log(LogLevelInfo, "server action receive");
    MessageParser parser;
    std::vector<IServerAction*> newActions = parser.parse(params[0], clientFd, cli);
    while (!newActions.empty()) {
        server->addNewAction(newActions.front());
        newActions.erase(newActions.begin());
    }
}
