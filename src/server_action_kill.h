/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_kill.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/05 14:22:29 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/05 16:11:41 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_KILL_H_
#define SERVER_ACTION_KILL_H_

#include <vector>
#include <string>

#include "connection.h"
#include "iserver_action.h"

class ServerActionKill: public IServerAction {
 public:
    ServerActionKill(
        std::vector<std::string> params, const int& clientFd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionKill() {}
 private:
    std::vector<std::string> params;
    Connection* connection;
    void handleServerKill();
    void handleClientDisconnect();
    void sendKillMessageToServer();
    void handleNoSuchNick();
    void handleClientKill();
    void handleNotEnoughParams();
    void handleNoPrivileges();
};

#endif  // SERVER_ACTION_KILL_H_
