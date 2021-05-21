/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_server.h                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 14:58:16 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/19 11:42:11 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_SERVER_H_
#define SERVER_ACTION_SERVER_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "connection.h"

class ServerActionServer : public IServerAction {
 public:
    ServerActionServer(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionServer() {}
 private:
    Connection* connection;
    std::vector<std::string> params;
    void handleServerFromServer() const;
    void handleServerRegistration() const;
    void handleAlreadyRegistered() const;
    void handleNeedMoreParams() const;
};

#endif
