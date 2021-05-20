/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_privmsg.h                           :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 13:36:58 by jules         #+#    #+#                 */
/*   Updated: 2021/05/20 13:51:50 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_PRIVMSG_H_
#define SERVER_ACTION_PRIVMSG_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "connection.h"

class ServerActionPrivmsg : public IServerAction {
 public:
	ServerActionPrivmsg(
		std::vector<std::string> params, const int& fd, const std::string& prefix);
	void execute();
	IServerAction* clone() const;
	~ServerActionPrivmsg() {}
 private:
	void sendMessages(const std::vector<std::pair<Connection*, std::string> >& recipients, bool broadcast, std::string broadcastTo) const;
	std::vector<std::pair<Connection*, std::string> > findMatchingConnections();
	void connectionNotRegistered() const;
 	std::vector<std::string> params;
	Connection* sender;
};

#endif  // SERVER_ACTION_PRIVMSG_H_
