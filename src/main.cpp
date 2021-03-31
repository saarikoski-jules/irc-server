/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:04:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 10:53:40 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "logger.h"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    Logger::changeLoggingDestinationTo("./logs/latest.log");
    Logger::log(LogLevelInfo, "Program started");
    return (0);
}
