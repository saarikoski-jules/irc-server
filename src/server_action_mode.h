/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_mode.h                              :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:09:50 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/01 11:26:03 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_MODE_H_
#define SERVER_ACTION_MODE_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "channel.h"
#include "server.h"

class ServerActionMode: public IServerAction {
 public:
    ServerActionMode(
        std::vector<std::string> params, const int& clientFd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionMode() {}
 private:
    void execByMode(char sign);
    Channel* getChannel(const std::string& name, const std::string& key);
    std::vector<std::string> params;
    void sendReplyToLocalClient(const std::string& message, const std::string& prefix = Server::serverName) const;
    bool modeO(char sign, const std::string& user);
    bool editMode(char sign, char mode);
    bool setLimit(char sign, const std::string& limit);
    bool setKey(char sign, const std::string& key);
    bool setBanMask(char sign, const std::string& mask);
    bool listBanMasks() const;
    void broadcastChannelModeIs(const std::string& modes, const std::string& channelName, const std::vector<std::string>& params) const;
    void sendUnknownModeReply(char c) const;
    void connectionNotRegistered() const;
	void changeMode();
	void displayModes() const;
	void changeUserMode();
	void sendUsersDontMatchReply(std::string cliNick);
	void sendUnknownUModeReply(char c, std::string cliNick);
	void sendUModeIsReply(std::string clientMode, std::string cliNick);
	Channel* chan;
    Connection* connection;
	std::string clientNick;
};

#endif  // SERVER_ACTION_MODE_H_
