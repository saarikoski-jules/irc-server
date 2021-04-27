/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_send.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/23 16:35:17 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 17:26:00 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_SEND_H_
#define SERVER_ACTION_SEND_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionSend : public IServerAction {
 public:
    ServerActionSend(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionSend() {}
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_SEND_H_
