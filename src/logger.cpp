/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   logger.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 17:32:03 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 15:04:43 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "logger.h"

#include <string>
#include <fstream>
#include <iostream>

enum LogLevel Logger::maxLogLevel = LogLevelInfo;
std::ofstream Logger::outputFile;

void Logger::changeMaxLogLevelTo(enum LogLevel const& maxLogLevel) {
    Logger::maxLogLevel = maxLogLevel;
}

void Logger::changeLoggingDestinationTo(std::string const& filename) {
    if (outputFile.is_open()) {
        outputFile.close();
    }
    outputFile.open(filename, std::ofstream::out);
}

void Logger::log(enum LogLevel const& logLevel, std::string const& message) {
    if (logLevel <= maxLogLevel) {
        if (outputFile.is_open() == true) {
            outputFile << message << std::endl;
        } else {
            std::cout << message << std::endl;
        }
    }
}
