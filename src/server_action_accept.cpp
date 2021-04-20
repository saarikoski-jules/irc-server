/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_accept.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:48:09 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 11:49:13 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_accept.h"

#include "server.h"

ServerActionAccept::ServerActionAccept(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) :
IServerAction(clientFd, 0, cli, prefix),
params(params) {}

void ServerActionAccept::execute() {
    // Logger::log(LogLevelInfo, "server action accept");
    server->acceptNewClient(clientFd);
}
