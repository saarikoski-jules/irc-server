/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/02 12:50:35 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/02 13:38:32 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H_
#define CLIENT_H_

struct Client {
    explicit Client(const int fd);
    const int fd;
};

#endif  // CLIENT_H_
