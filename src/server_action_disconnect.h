/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_disconnect.h                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:43:34 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/26 13:23:47 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_DISCONNECT_H_
#define SERVER_ACTION_DISCONNECT_H_

#include <string>
#include <vector>

#include "iserver_action.h"
#include "connection.h"

class ServerActionDisconnect: public IServerAction {
 public:
    ServerActionDisconnect(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionDisconnect() {}
 private:
    void disconnectServer();
    void handleSplitClients();
    void handleSplitServer();
    void disconnectClient();
    void disconnectNoType();
    Connection* connection;
    std::string disconnectMessage;
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_DISCONNECT_H_
