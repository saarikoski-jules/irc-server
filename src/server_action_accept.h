/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_accept.h                            :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:45:40 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/27 13:20:43 by jules        ########   odam.nl          */
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
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionAccept() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_ACCEPT_H_
