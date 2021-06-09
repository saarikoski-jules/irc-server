/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.cpp                                    :+:    :+:             */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 16:07:44 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/09 13:20:28 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "connection.h"

#include <string>
#include <stdexcept>

Connection::Connection() :
fd(-1),
connectionType(Connection::NoType),
partialMsg("") {
}

Connection::Connection(const int& fd) :
fd(fd),
connectionType(Connection::NoType),
partialMsg("") {
}

Connection::Connection(const std::string& serverConfiguration) :
fd(-1),
connectionType(Connection::NoType),
server(serverConfiguration),
partialMsg("") {
}

Connection::~Connection() {
    while (leafConnections.size() > 0) {
        delete leafConnections.at(0);
        leafConnections.erase(leafConnections.begin());
    }
}

bool matchPrefix(const std::string& prefix, const std::string& nick) {
	std::string::const_iterator endOfNick = prefix.begin();
	for (; endOfNick != prefix.end(); endOfNick++) {
		if ((*endOfNick > '9' || *endOfNick < '0')
        && (*endOfNick > 'z' || *endOfNick < 'a')
        && (*endOfNick > 'Z' || *endOfNick < 'A')) {
			break;
		}
	}
	std::string nickFromPrefix(prefix.begin(), endOfNick);
	if (nickFromPrefix == nick) {
		return (true);
	}
	return (false);
}

Connection* Connection::getLeafConnection(const std::string& str) {
	for (std::vector<Connection*>::iterator i = leafConnections.begin(); i != leafConnections.end(); i++) {
		if ((*i)->connectionType == ClientType && matchPrefix(str, (*i)->client.nickName)) {
			return (*i);
		}
	}
	throw std::out_of_range("Coundn't find matching connection in leaves");
}

void Connection::removeLeafConnectionByNick(const std::string& nickname) {
	for (std::vector<Connection*>::iterator i = leafConnections.begin(); i != leafConnections.end(); i++) {
		if ((*i)->connectionType == ClientType && (*i)->client.nickName == nickname) {
			delete *i;
			leafConnections.erase(i);
            return ;
		}
	}
	throw std::out_of_range("Coundn't find matching connection in leaves");
}

void Connection::removeLeafServerByName(const std::string& serverName) {
	for (std::vector<Connection*>::iterator i = leafConnections.begin(); i != leafConnections.end(); i++) {
		if ((*i)->connectionType == Connection::ServerType && (*i)->server.name == serverName) {
			delete *i;
			leafConnections.erase(i);
            return ;
		}
	}
	throw std::out_of_range("Coundn't find matching connection in leaves");
}
