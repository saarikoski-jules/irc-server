/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_store.h                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/06/09 12:13:53 by jules        #+#    #+#                  */
/*   Updated: 2021/06/09 12:35:14 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_STORE_H_
#define SERVER_ACTION_STORE_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionStore: public IServerAction {
 public:
 	ServerActionStore(
		std::vector<std::string> params, const int& fd, const std::string& prefix = "");
	void execute();
	IServerAction* clone() const;
	~ServerActionStore() {}
 private:
 	std::vector<std::string> params;
};

#endif  // SERVER_ACTION_STORE_H_

