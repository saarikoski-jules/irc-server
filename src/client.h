/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 12:50:35 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/30 17:15:43 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>

struct Client {
    Client();
    friend std::ostream& operator << (std::ostream& out, const Client& client);
    std::string nickName;
    std::string userName;
    std::string hostName;
    std::string serverName;
    std::string realName;
    std::string mode;
    bool registered;
};

#endif  // CLIENT_H_
