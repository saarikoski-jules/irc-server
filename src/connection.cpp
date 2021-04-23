/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 16:07:44 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/21 13:39:59 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "connection.h"

#include <string>

Connection::Connection() :
fd(-1),
connectionType(Connection::NoType) {
}

Connection::Connection(const int& fd) :
fd(fd),
connectionType(Connection::NoType) {
}

Connection::Connection(const std::string& serverConfiguration) :
fd(-1),
connectionType(Connection::ServerType),
server(serverConfiguration) {
}
