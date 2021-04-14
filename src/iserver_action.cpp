/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:15 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/14 16:10:11 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "iserver_action.h"

#include <vector>

#include "client.h"

Server* IServerAction::server = NULL;

IServerAction::IServerAction(
    const int& clientFd, const size_t& requiredParams, const Client* cli, const std::string& prefix) :
clientFd(clientFd),
prefix(prefix),
requiredParams(requiredParams),
cli(cli) {}
