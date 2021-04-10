/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:15 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/10 13:41:18 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "iserver_action.h"

#include <vector>

#include "client.h"

IServerAction::IServerAction(
    const int& clientFd, const size_t& requiredParams, const std::string& prefix) :
clientFd(clientFd),
prefix(prefix),
requiredParams(requiredParams) {}
