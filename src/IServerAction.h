/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IServerAction.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:14 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 13:28:29 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERACTION_H_
#define ISERVERACTION_H_

#include <vector>

#include "client.h"
// #include "action_factory.h"


class IServerAction {
 public:
    IServerAction(const std::vector<Client>& clients);
    virtual void execute() = 0;
    virtual ~IServerAction() {};
 protected:
    const std::vector<Client>& clients;
};

#endif  //  ISERVERACTION_H_
