/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_quit.cpp                            :+:    :+:             */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/07 14:10:01 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/09 16:56:09 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_quit.h"

#include <vector>
#include <unistd.h>

#include "message_parser.h"
#include "server.h"
#include "logger.h"
#include "server_action_disconnect.h"

ServerActionQuit::ServerActionQuit(
    std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 0, prefix),
params(params) {}

void ServerActionQuit::execute() {
    connection = server->getConnectionByFd(fd);
    switch (connection->connectionType)
    {
    case Connection::ServerType:
        handleServerQuit();
        break;
    case Connection::ClientType:
    case Connection::NoType:
        handleClientQuit();
        break;
    }
}

void ServerActionQuit::handleServerQuit() {
    try {
		Connection* leaf = connection->getLeafConnection(prefix);
        if (params.size() >= 1) {
            quitMessage = params[0];
        } else {
            quitMessage = "EOF from client";
        }
		std::string reply("QUIT :" + quitMessage);
		server->sendMessageToAllLocalUsersInClientChannels(leaf, reply, prefix);
		server->removeClientFromChannels(leaf);
		connection->removeLeafConnectionByNick(prefix);
        server->sendMessageToAllServersButOne(reply + "\r\n", fd);
    } catch (const std::exception& e) {
		Logger::log(LogLevelDebug, "handleServerQuit");
		Logger::log(LogLevelError, e.what());
    }
}

void ServerActionQuit::handleClientQuit() {
    try {
    	server->addNewAction(new ServerActionDisconnect(params, fd, prefix));
	} catch (const std::exception& e) {
        Logger::log(LogLevelError, e.what());
    }
}

IServerAction* ServerActionQuit::clone() const {
    return (new ServerActionQuit(*this));
}
