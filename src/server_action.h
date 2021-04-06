/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:41:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/06 14:14:00 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_H_
#define SERVER_ACTION_H_

#include <string>

struct ServerAction {
    enum Type {
        NO_ACTION,
        NEW_CLIENT,
        NEW_MESSAGE,
        DISCONNECT_CLIENT
    };

    ServerAction();
    Type type;
    int clientFd;
    std::string message;
};

#include "IServerAction.h"
#include "action_factory.h"

class SserverAction: public IServerAction {
 public:
    SserverAction(const std::vector<Client>& clients);
    // actionType type;
    void execute();
    ~SserverAction() {};
};

class ServerActionNick: public IServerAction {
 public:
    ServerActionNick(const std::vector<Client>& clients, std::vector<std::string> params);
    // actionType type;
    void execute();
    ~ServerActionNick() {};
 private:
    std::vector<std::string> params;
};

#endif  // SERVER_ACTION_H_
