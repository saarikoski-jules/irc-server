/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_privmsg.h                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/04/28 13:36:58 by jules        #+#    #+#                  */
/*   Updated: 2021/04/28 14:26:34 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_PRIVMSG_H_
#define SERVER_ACTION_PRIVMSG_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionPrivmsg : public IServerAction {
 public:
	ServerActionPrivmsg(
		std::vector<std::string> params, const int& fd, const std::string& prefix);
	void execute();
	IServerAction* clone() const;
	~ServerActionPrivmsg() {}
 private:
 	std::vector<std::string> params;
};

#endif  // SERVER_ACTION_PRIVMSG_H_
