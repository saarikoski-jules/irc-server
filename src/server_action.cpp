/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/07 10:41:07 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action.h"

#include "logger.h"
#include "server.h"

ServerActionNick::ServerActionNick(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionNick::execute(Server*) {
    Logger::log(LogLevelInfo, "server action nick");
}

ServerActionAccept::ServerActionAccept(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionAccept::execute(Server* server) {
    Logger::log(LogLevelInfo, "server action accept");
    server->acceptNewClient(clientFd);
}

ServerActionReceive::ServerActionReceive(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionReceive::execute(Server* server) {
    Logger::log(LogLevelInfo, "server action receive");
    MessageParser parser;
    std::vector<IServerAction*> newActions = parser.parse(params[0], clientFd);
    while (!newActions.empty()) {
        server->addNewAction(newActions.front());
        newActions.erase(newActions.begin());
    }
}

ServerActionDisconnect::ServerActionDisconnect(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionDisconnect::execute(Server* server) {
    Logger::log(LogLevelInfo, "server action disconnect");
    server->deleteClient(clientFd);
}
