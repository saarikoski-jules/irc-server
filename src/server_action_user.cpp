/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_user.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:34:39 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 11:35:12 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_user.h"

#include <iostream>

#include "server.h"
#include "logger.h"

ServerActionUser::ServerActionUser(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 4, cli, prefix),
params(params) {}

void ServerActionUser::execute() {
    Logger::log(LogLevelInfo, "server action accept");
    try {
        const std::string& newUserName = params[0];
        const std::string& newHostName = params[1];
        const std::string& newServerName = params[2];
        const std::string& newRealName = params[3];
        if (server->usernameExists(newUserName) == false) {
            cli->userName = newUserName;
            cli->hostName = newHostName;
            cli->serverName = newServerName;
            cli->realName = newRealName;
            if (cli->nickName != "*") {
                cli->registered = true;
            }
            std::cout << *cli << std::endl;
        } else {
            Logger::log(LogLevelError, "Something went wrong while setting user info");
            // server->sendErrorToClient(ERR_NONICKNAMEGIVEN, clientFd);
        }
        // TODO(Jelle) Handle server error's
        // TODO(Jelle) Handle server2server communication
    } catch (const std::out_of_range& e) {
        // TODO(Jelle) Handle non valid client fd
    }
}
