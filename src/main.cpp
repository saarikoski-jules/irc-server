/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:04:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 11:24:30 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    Socket socket;
    socket.open();

    return (0);
}
