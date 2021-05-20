/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   server_action_part.h                              :+:    :+:             */
/*                                                    +:+                     */
/*   By: jules <jsaariko@student.codam.nl>           +#+                      */
/*                                                  +#+                       */
/*   Created: 2021/05/20 16:11:02 by jules        #+#    #+#                  */
/*   Updated: 2021/05/20 16:17:09 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_PART_H_
#define SERVER_ACTION_PART_H_

#include <string>
#include <vector>

#include "iserver_action.h"
#include "server.h"

class ServerActionPart: public IServerAction {
 public:
 	ServerActionPart(
		std::vector<std::string> params, const int& fd, const std::string& prefix = "");
	void execute();
	IServerAction* clone() const;
	~ServerActionPart() {}
 private:
 	std::vector<std::string> params;
};

#endif  // SERVER_ACTION_PART_H_

