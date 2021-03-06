/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_topic.h                             :+:    :+:             */
/*                                                     +:+                    */
/*   By: jules <jsaariko@student.codam.nl>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/07 15:20:56 by jules         #+#    #+#                 */
/*   Updated: 2021/06/02 11:12:50 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_TOPIC_H_
#define SERVER_ACTION_TOPIC_H_

#include <vector>
#include <string>

#include "server.h"
#include "iserver_action.h"
#include "channel.h"
#include "connection.h"

class ServerActionTopic: public IServerAction {
 public:
    ServerActionTopic(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionTopic() {}
 private:
    std::vector<std::string> params;
    Connection* connection;
    Channel* chan;
    void checkTopic(const std::string& clientNick) const;
    void changeTopic(const std::string& clientNick);
    void sendReplyToLocalClient(const std::string& message, const std::string& prefix = Server::serverName) const;
};

#endif  // SERVER_ACTION_TOPIC_H_
