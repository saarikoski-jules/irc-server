/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 12:50:28 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/07 16:39:52 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

#include <iostream>

Client::Client(const int fd) :
fd(fd),
registered(false) {
}

Client::Client(const Client& client) :
fd(client.fd),
registered(false) {
    *this = client;
}

Client& Client::operator = (const Client& client) {
    nickName = client.nickName;
    return (*this);
}

std::ostream& operator << (std::ostream& out, const Client& client) {
    std::cout << "Client (" << std::endl
        << "Nick name:   " << client.nickName << std::endl
        << "User name:   " << client.userName << std::endl
        << "Server name: " << client.serverName << std::endl
        << "Host name:   " << client.hostName << std::endl
        << "Real name:   " << client.realName << std::endl
        << ")" << std::endl;
    return (out);
}
