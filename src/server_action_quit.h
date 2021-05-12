/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action_quit.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/07 14:12:44 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/07 15:17:07 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_QUIT
#define SERVER_ACTION_QUIT

#include <vector>
#include <string>

#include "iserver_action.h"
#include "connection.h"

class ServerActionQuit: public IServerAction {
 public:
     ServerActionQuit(
        std::vector<std::string> params, const int& fd, const std::string& prefix = "");
    void execute();
    IServerAction* clone() const;
    ~ServerActionQuit() {}
 private:
    Connection* connection;
    std::string quitMessage;
    std::vector<std::string> params;
    void handleServerQuit();
    void handleClientQuit();
};

#endif  // SERVER_ACTION_QUIT
