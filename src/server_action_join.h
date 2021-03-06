/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_join.h                              :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:18:00 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/01 09:31:36 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_JOIN_H_
#define SERVER_ACTION_JOIN_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "channel.h"
#include "server.h"

class ServerActionJoin: public IServerAction {
 public:
    ServerActionJoin(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionJoin() {}
 private:
	Connection* getActualClient();
	void joinChannels();
    void addClientToChannel(const std::string& name, const std::string& key);
    void addExternalClientToChannel();
    void connectionNotRegistered() const;
    void handleNeedMoreParams() const;
    void broadcastJoin(Channel* chan);
    void sendToLocalClient(const std::string& message, const std::string& prefix = Server::serverName) const;
	void sendNames(Channel* chan);
	std::vector<std::string> params;
    Connection* connection;
	std::string clientNick;
};

#endif  // SERVER_ACTION_JOIN_H_
