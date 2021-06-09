/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.h                                      :+:    :+:             */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 16:03:42 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/09 12:31:31 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <string>
#include <vector>

#include "client.h"
#include "server_connection.h"

struct Connection {
    Connection();
    Connection(const int& fd);
    Connection(const std::string& serverConfiguration);
    ~Connection();
    Connection* getLeafConnection(const std::string& str);
    void removeLeafConnectionByNick(const std::string& str);
    void removeLeafServerByName(const std::string& serverName);
    int fd;
    enum {
        ClientType,
        ServerType,
        NoType
    } connectionType;
    std::string password;
    Client client;
    ServerConnection server;
    std::vector<Connection*> leafConnections;
    bool isStartingServer;
	std::string partialMsg;
};

#endif  // CONNECTION_H_
