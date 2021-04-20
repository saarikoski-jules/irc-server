/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_receive.h                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:37:56 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:25:32 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_RECEIVE_H_
#define SERVER_ACTION_RECEIVE_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionReceive: public IServerAction {
 public:
    ServerActionReceive(
        std::vector<std::string> params, const int& clientFd,
        Client* client, const std::string& prefix = "");
    void execute();
    ~ServerActionReceive() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_RECEIVE_H_
