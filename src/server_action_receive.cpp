/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_receive.cpp                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:38:56 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/09 13:16:28 by jules        ########   odam.nl          */
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
	Connection* user = server->getConnectionByFd(fd);
	std::string fullMsg(user->partialMsg + params[0]);
	user->partialMsg = "";
	std::vector<IServerAction*> newActions = parser.parse(fullMsg, fd);
    while (!newActions.empty()) {
        server->addNewAction(newActions.front());
        newActions.erase(newActions.begin());
    }
}

IServerAction* ServerActionReceive::clone() const {
    return (new ServerActionReceive(*this));
}
