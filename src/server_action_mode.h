/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_mode.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:09:50 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 14:27:27 by jsaariko      ########   odam.nl         */
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
    Channel* getChannel(const std::string& name, const std::string& key);
    void joinServer(const std::string& name, const std::string& key);
    std::vector<std::string> params;
    void modeO(char sign, const std::string& user);
    void editMode(char sign, char mode);
    void setLimit(char sign, const std::string& limit);
    void setKey(char sign, const std::string& key);
    void sendChannelModeIsReply() const;
    void sendUnknownModeReply(char c) const;
    void setBanMask(char sign, const std::string& mask);
    void listBanMasks() const;
    Channel* chan;
};

#endif  // SERVER_ACTION_MODE_H_
