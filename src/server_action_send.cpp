/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_send.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/23 16:35:08 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 17:25:50 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_send.h"

#include <vector>
#include <string>

#include "server.h"

ServerActionSend::ServerActionSend(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionSend::execute() {
    try {
        server->sendMessage(fd, params[0]);
    } catch (const ServerException& e) {
        server->delayFirstAction();
    }
}

IServerAction* ServerActionSend::clone() const {
    return (new ServerActionSend(*this));
}
