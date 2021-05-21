/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.h                                      :+:    :+:             */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 16:03:42 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/21 12:05:10 by jules        ########   odam.nl          */
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
    Connection* getLeafConnection(const std::string& str);
    void removeLeafConnectionByNick(const std::string& str);
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
};

#endif  // CONNECTION_H_
