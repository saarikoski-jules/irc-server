/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_join.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:18:00 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:29:48 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_JOIN_H_
#define SERVER_ACTION_JOIN_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "channel.h"

class ServerActionJoin: public IServerAction {
 public:
    ServerActionJoin(
        std::vector<std::string> params, const int& clientFd,
        Client* cli, const std::string& prefix = "");
    void execute();
    ~ServerActionJoin() {}
 private:
    Channel* getChannel(const std::string& name, const std::string& key);
    void joinServer(const std::string& name, const std::string& key);
    void handleNeedMoreParams() const;
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_JOIN_H_
