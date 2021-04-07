/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:14 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/07 10:23:31 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVER_ACTION_H_
#define ISERVER_ACTION_H_

#include <vector>

// #include "server.h"
class Server;

class IServerAction {
 public:
    explicit IServerAction(const int& clientFd);
    virtual void execute(Server*) = 0;
    virtual ~IServerAction() {}
 protected:
    const int clientFd;
};

#endif  //  ISERVER_ACTION_H_