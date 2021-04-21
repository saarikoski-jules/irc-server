/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_mode.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:09:50 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/21 11:27:11 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_MODE_H_
#define SERVER_ACTION_MODE_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "channel.h"

class ServerActionMode: public IServerAction {
 public:
    ServerActionMode(
        std::vector<std::string> params, const int& clientFd,
        Client* cli, const std::string& prefix = "");
    void execute();
    ~ServerActionMode() {}
 private:
    void execByMode(char sign);
    Channel* getChannel(const std::string& name, const std::string& key);
    std::vector<std::string> params;
    bool modeO(char sign, const std::string& user);
    bool editMode(char sign, char mode);
    bool setLimit(char sign, const std::string& limit);
    bool setKey(char sign, const std::string& key);
    bool setBanMask(char sign, const std::string& mask);
    bool listBanMasks() const;
    void sendChannelModeIsReply(const std::string& modes, const std::string& channelName, const std::vector<std::string>& params) const;
    void sendUnknownModeReply(char c) const;
    Channel* chan;
};

#endif  // SERVER_ACTION_MODE_H_
