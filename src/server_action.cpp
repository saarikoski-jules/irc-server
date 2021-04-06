/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:45:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/06 17:53:38 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server_action.h"

#include "logger.h"

ServerActionNick::ServerActionNick(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionNick::execute(std::vector<Client>&) {
    Logger::log(LogLevelInfo, "server action nick");
}

ServerActionAccept::ServerActionAccept(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionAccept::execute(std::vector<Client>& clients) {
    Logger::log(LogLevelInfo, "server action accept");
    clients.push_back(Client(clientFd));
}

ServerActionReceive::ServerActionReceive(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionReceive::execute(std::vector<Client>&) {
    Logger::log(LogLevelInfo, "server action receive");
}

ServerActionDisconnect::ServerActionDisconnect(std::vector<std::string> params, const int& clientFd) :
IServerAction(clientFd),
params(params) {}

void ServerActionDisconnect::execute(std::vector<Client>& clients) {
    Logger::log(LogLevelInfo, "server action disconnect");
    std::vector<Client>::iterator it = clients.begin();
    while (it != clients.end()) {
        if ((*it).fd == clientFd) {
            Logger::log(LogLevelInfo, "Client disconnected");
            clients.erase(it);
            break;
        }
        it++;
    }
}
