/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_receive.cpp                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:38:56 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/27 12:45:37 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_receive.h"

#include <vector>

#include "message_parser.h"
#include "server.h"

ServerActionReceive::ServerActionReceive(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionReceive::execute() {
    MessageParser parser;
    std::vector<IServerAction*> newActions = parser.parse(params[0], fd);
    while (!newActions.empty()) {
        server->addNewAction(newActions.front());
        newActions.erase(newActions.begin());
    }
}

IServerAction* ServerActionReceive::clone() const {
    return (new ServerActionReceive(*this));
}
