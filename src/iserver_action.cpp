/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   iserver_action.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:15 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/21 13:31:49 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "iserver_action.h"

#include <vector>

#include "client.h"

Server* IServerAction::server = NULL;

IServerAction::IServerAction(
    const int& fd, const size_t& requiredParams, const std::string& prefix) :
fd(fd),
prefix(prefix),
requiredParams(requiredParams) {
}
