/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:14 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/10 13:11:30 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVER_ACTION_H_
#define ISERVER_ACTION_H_

#include <vector>
#include <string>

class Server;

class IServerAction {
 public:
    IServerAction(
        const int& clientFd, const size_t& requiredParams, const std::string& prefix);
    virtual void execute(Server*) = 0;
    virtual ~IServerAction() {}
 protected:
    const int clientFd;
    const std::string prefix;
    const size_t requiredParams;
};

#endif  //  ISERVER_ACTION_H_
