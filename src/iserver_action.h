/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:14 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 17:52:27 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERACTION_H_
#define ISERVERACTION_H_

#include <vector>

#include "client.h"

class IServerAction {
 public:
    IServerAction(const int& clientFd);
    virtual void execute(std::vector<Client>&) = 0;
    virtual ~IServerAction() {};
 protected:
    const int clientFd;
};

#endif  //  ISERVERACTION_H_
