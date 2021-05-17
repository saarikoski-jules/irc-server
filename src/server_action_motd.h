/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_motd.h                              :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/07 12:00:51 by jules        #+#    #+#                  */
/*   Updated: 2021/05/07 13:12:21 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_MOTD_H_
#define SERVER_ACTION_MOTD_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionMotd: public IServerAction {
 public:
	ServerActionMotd(
		std::vector<std::string> params, const int& fd, const std::string& prefix = "");
	void execute();
	IServerAction* clone() const;
	~ServerActionMotd() {}
 private:
 	std::vector<std::string> params;
};

#endif  // SERVER_ACTION_MOTD_H_

