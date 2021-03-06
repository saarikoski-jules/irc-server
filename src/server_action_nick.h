/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_nick.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:50:04 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/06/02 15:06:09 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_NICK_H_
#define SERVER_ACTION_NICK_H_

#include <vector>
#include <string>

#include "connection.h"
#include "iserver_action.h"

class ServerActionNick: public IServerAction {
 public:
    ServerActionNick(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    IServerAction* clone() const;
    void execute();
    ~ServerActionNick() {}
 private:
    void handleServerNick();
    void handleNickNameCollision() const;
    void sendKillMessageToServer() const;
    void scheduleConnectionCollisionKill() const;
    void handleClientNick();
    void handleInvalidPassword() const;
    void handleNickNameChange() const;
    void handleNickNameInUse() const;
    void handleNoNicknameGiven() const;
    void handleErroneusNickName() const;
    Connection* connection;
    std::vector<std::string> params;
    const std::string* newNickName;
};

#endif  // SERVER_ACTION_NICK_H_
