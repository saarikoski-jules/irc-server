/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_disconnect.cpp                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:43:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 11:44:48 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_disconnect.h"

#include "server.h"

ServerActionDisconnect::ServerActionDisconnect(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionDisconnect::execute() {
    server->deleteConnection(fd);
}
