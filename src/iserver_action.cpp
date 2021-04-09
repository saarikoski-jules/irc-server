/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:15 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/09 18:37:38 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "iserver_action.h"

#include <vector>

#include "client.h"

IServerAction::IServerAction(const int& clientFd, const size_t& requiredParams) :
clientFd(clientFd),
prefix(""),
requiredParams(requiredParams) {}

IServerAction::IServerAction(const int& clientFd, const std::string& prefix, const size_t& requiredParams) :
clientFd(clientFd),
prefix(prefix),
requiredParams(requiredParams) {}
