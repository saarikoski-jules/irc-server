/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_store.cpp                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/06/09 12:09:02 by jules        #+#    #+#                  */
/*   Updated: 2021/06/09 13:42:02 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_store.h"

#include <vector>
#include <string>

#include "server.h"

ServerActionStore::ServerActionStore(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {}

void ServerActionStore::execute() {
	try {
		Connection* user = server->getConnectionByFd(fd);
		user->partialMsg = params[0];
	} catch (const std::exception& e) {
		// client doesn't exist, can't store data
	}
}

IServerAction* ServerActionStore::clone() const {
	return (new ServerActionStore(*this));
}
