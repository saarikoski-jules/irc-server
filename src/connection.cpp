/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 16:07:44 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/04 13:22:57 by jsaariko      ########   odam.nl         */
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
connectionType(Connection::NoType),
server(serverConfiguration) {
}

Connection* Connection::getLeafConnection(const std::string& str) {
	for (std::vector<Connection>::iterator i = leafConnections.begin(); i != leafConnections.end(); i++) {
		//TODO(Jules): Make a better matching function
		if (i->connectionType == ClientType && i->client.nickName == str) {
			return (&*i);
		}
		//TOOD(Jules): also check other server connections
	}
	throw std::out_of_range("Coundn't find matching connection in leaves");
}
