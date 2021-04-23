/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_disconnect.h                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:43:34 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:30:42 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_DISCONNECT_H_
#define SERVER_ACTION_DISCONNECT_H_

#include <string>
#include <vector>

#include "iserver_action.h"

class ServerActionDisconnect: public IServerAction {
 public:
    ServerActionDisconnect(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
 void execute();
    ~ServerActionDisconnect() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_DISCONNECT_H_
