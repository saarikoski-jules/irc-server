/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_squit.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/26 15:40:57 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/28 12:13:11 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_SQUIT_H_
#define SERVER_ACTION_SQUIT_H_

#include <vector>
#include <string>

#include "iserver_action.h"
#include "connection.h"

class ServerActionSquit: public IServerAction {
 public:
     ServerActionSquit(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionSquit() {}
 private:
    std::vector<std::string> params;
    Connection* connection;
    std::string quitMessage;
    void handleServerSquit();
};

#endif  // SERVER_ACTION_SQUIT_H_
