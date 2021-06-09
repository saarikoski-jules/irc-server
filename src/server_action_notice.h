/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_notice.h                            :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 13:36:58 by jules         #+#    #+#                 */
/*   Updated: 2021/06/09 13:58:17 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_NOTICE_H_
#define SERVER_ACTION_NOTICE_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "connection.h"

class ServerActionNotice : public IServerAction {
 public:
	ServerActionNotice(
		std::vector<std::string> params, const int& fd, const std::string& prefix);
	void execute();
	IServerAction* clone() const;
	~ServerActionNotice() {}
 private:
	void sendMessages(const std::vector<std::pair<Connection*, std::string> >& recipients, bool broadcast, std::string broadcastTo) const;
	void findMatchingConnections();
 	std::vector<std::string> params;
	Connection* sender;
};

#endif  // SERVER_ACTION_NOTICE_H_
