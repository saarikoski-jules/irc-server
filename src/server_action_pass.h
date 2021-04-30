/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_pass.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/28 11:50:17 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/28 14:39:21 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_PASS_H_
#define SERVER_ACTION_PASS_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "connection.h"

class ServerActionPass : public IServerAction {
 public:
    ServerActionPass(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionPass() {}
 private:
    Connection* connection;
    std::vector<std::string> params;
    void handlePass() const;
    void handleAlreadyRegistered() const;
    void handleNeedMoreParams() const;
};

#endif  // SERVER_ACTION_PASS_H_
