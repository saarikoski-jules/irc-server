/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_nick.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 11:50:04 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/03 13:32:44 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_NICK_H_
#define SERVER_ACTION_NICK_H_

#include <vector>
#include <string>

#include "iserver_action.h"

class ServerActionNick: public IServerAction {
 public:
    ServerActionNick(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    IServerAction* clone() const;
    void execute();
    ~ServerActionNick() {}
 private:
    void handleNickNameChange() const;
    void handleNickNameInUse() const;
    void handleNoNicknameGiven() const;
    void handleErroneusNickName() const;
    std::vector<std::string> params;
    const std::string* newNickName;
};

#endif  // SERVER_ACTION_NICK_H_
