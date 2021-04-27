/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_receive.h                           :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:37:56 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/27 12:46:06 by jules        ########   odam.nl          */
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
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionReceive() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_RECEIVE_H_
