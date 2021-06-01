/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_who.h                               :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/06/01 11:58:54 by jules        #+#    #+#                  */
/*   Updated: 2021/06/01 12:17:09 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_WHO_H_
#define SERVER_ACTION_WHO_H_

#include "iserver_action.h"

#include <string>
#include <vector>

class ServerActionWho: public IServerAction {
 public:
	ServerActionWho(
		std::vector<std::string> params, const int& clientFd, const std::string& prefix);
	void execute();
	IServerAction* clone() const;
	~ServerActionWho() {}
 private:
 	std::vector<std::string> params;
};

#endif  // SERVER_ACTION_WHO_H_

