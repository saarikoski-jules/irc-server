/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_names.h                             :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/05 11:34:37 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/01 15:55:31 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_NAMES_H_
#define SERVER_ACTION_NAMES_H_

#include "iserver_action.h"
#include "connection.h"

class ServerActionNames: public IServerAction {
 public:
	ServerActionNames(std::vector<std::string> params, const int& fd, const std::string& prefix = "");
	void execute();
	IServerAction* clone() const;
	~ServerActionNames() {}
 private:
	void namesReply(const std::string& channelName, const std::string& names, const std::string& rights) const;
	void endOfNamesReply(const std::string& channelName) const;
	std::vector<std::string> params;
	Connection* connection;
};

#endif  // SERVER_ACTION_NAMES_H_
