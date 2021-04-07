/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_action.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 10:41:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/02 18:55:52 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_ACTION_H_
#define SERVER_ACTION_H_

#include <string>

struct ServerAction {
    enum Type {
        NO_ACTION,
        NEW_CLIENT,
        NEW_MESSAGE,
        DISCONNECT_CLIENT
    };

    ServerAction();
    Type type;
    int clientFd;
    std::string message;
};

#endif  // SERVER_ACTION_H_
