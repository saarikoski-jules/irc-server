/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:14 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/14 17:22:07 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVER_ACTION_H_
#define ISERVER_ACTION_H_

#include <vector>
#include <string>

#include "client.h"

class Server;

class IServerAction {
 public:
    IServerAction(
        const int& clientFd, const size_t& requiredParams, Client* cli, const std::string& prefix);
    virtual void execute() = 0;
    virtual ~IServerAction() {}
    static Server* server;
 protected:
    const int clientFd;
    const std::string prefix;
    const size_t requiredParams;
    Client* cli;
};

#endif  //  ISERVER_ACTION_H_
