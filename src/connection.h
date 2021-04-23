/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 16:03:42 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/21 19:00:22 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <string>

#include "client.h"
#include "server_connection.h"

struct Connection {
    Connection();
    Connection(const int& fd);
    Connection(const std::string& serverConfiguration);
    int fd;
    enum {
        ClientType,
        ServerType,
        NoType
    } connectionType;
    Client client;
    ServerConnection server;
};

#endif  // CONNECTION_H_