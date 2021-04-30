/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_ping.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 17:01:54 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/28 18:03:09 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_PING_H_
#define SERVER_ACTION_PING_H_

#include <vector>
#include <string>

#include "connection.h"
#include "iserver_action.h"

class ServerActionPing: public IServerAction {
 public:
    ServerActionPing(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionPing() {}
 private:
    Connection* connection;
    std::vector<std::string> params;
    void handlePingClient() const;
    void handlePingServer() const;
    void handleNotRegistered() const;
    void handleNotEnoughParams() const;
};

#endif  // SERVER_ACTION_PING_H_
