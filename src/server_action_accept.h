/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_accept.h                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:45:40 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:31:43 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_ACCEPT_H_
#define SERVER_ACTION_ACCEPT_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionAccept: public IServerAction {
 public:
    ServerActionAccept(
        std::vector<std::string> params, const int& clientFd,
        Client* client, const std::string& prefix = "");
    void execute();
    ~ServerActionAccept() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_ACCEPT_H_
