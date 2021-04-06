/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/06 15:09:31 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action.h"

ServerAction::ServerAction() :
type(NO_ACTION) {
}

SserverAction::SserverAction(const std::vector<Client>& clients) :
IServerAction(clients) {}

void SserverAction::execute() {
}

ServerActionNick::ServerActionNick(const std::vector<Client>& clients, std::vector<std::string> params) :
IServerAction(clients), 
params(params) {}

#include "logger.h"

void ServerActionNick::execute() {
    Logger::log(LogLevelInfo, params.front());
    
}
