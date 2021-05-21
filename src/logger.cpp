/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   logger.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 17:32:03 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/19 12:41:59 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "logger.h"

#include <string>
#include <iostream>

enum LogLevel Logger::maxLogLevel = LogLevelInfo;

void Logger::changeMaxLogLevelTo(enum LogLevel const& maxLogLevel) {
    Logger::maxLogLevel = maxLogLevel;
}

void Logger::log(enum LogLevel const& logLevel, std::string const& message) {
    if (logLevel <= maxLogLevel) {
        std::cout << message << std::endl;
    }
}
