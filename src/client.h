/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 12:50:35 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/07 16:39:28 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>

struct Client {
    explicit Client(const int fd);
    explicit Client(const Client& client);
    Client& operator = (const Client& client);
    friend std::ostream& operator << (std::ostream& out, const Client& client);
    const int fd;
    std::string nickName;
    std::string userName;
    std::string hostName;
    std::string serverName;
    std::string realName;
    bool registered;
};

#endif  // CLIENT_H_
