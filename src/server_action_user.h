/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_user.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:31:21 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:25:30 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_USER_H_
#define SERVER_ACTION_USER_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionUser: public IServerAction {
 public:
    ServerActionUser(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    ~ServerActionUser() {}
 private:
    std::vector<std::string> params;
    const std::string* newUserName;
    const std::string* newHostName;
    const std::string* newServerName;
    const std::string* newRealName;
};

#endif  // SERVER_ACTION_USER_H_
