/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_part.cpp                            :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/20 16:04:13 by jules        #+#    #+#                  */
/*   Updated: 2021/05/20 16:16:15 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "server_action_part.h"

#include <string>
#include <vector>

#include "construct_reply.h"

ServerActionPart::ServerActionPart(
	std::vector<std::string> params, const int& fd, const std::string& prefix) :
IServerAction(fd, 1, prefix),
params(params) {
	if (params.size() < requiredParams) {
		Connection* connection = server->getConnectionByFd(fd);
		if (server->hasLocalConnection(*connection)) {
			server->sendReplyToClient(fd, constructNeedMoreParamsReply(connection->client.nickName, "PART"));
		}
		throw std::invalid_argument("Not enough params");
	}
}

void ServerActionPart::execute() {

}

IServerAction* ServerActionPart::clone() const {
	return (new ServerActionPart(*this));
}
