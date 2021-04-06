/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   IServerAction.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:44:15 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 12:39:37 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "IServerAction.h"

#include <vector>

#include "client.h"

IServerAction::IServerAction(const std::vector<Client>& clients) :
clients(clients) {}
//TODO: change file name
