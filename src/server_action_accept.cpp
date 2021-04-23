/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_accept.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:48:09 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:32:03 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_accept.h"

#include <string>
#include <vector>

#include "server.h"

ServerActionAccept::ServerActionAccept(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionAccept::execute() {
    server->acceptNewConnection(fd);
}

