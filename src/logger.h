/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   logger.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 17:23:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 10:48:56 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <fstream>
#include <iostream>

enum LogLevel {
    LogLevelFatal,
    LogLevelError,
    LogLevelInfo,
    LogLevelDebug
};

class Logger {
 public:
    ~Logger();
    static void changeMaxLogLevelTo(const enum LogLevel& maxLogLevel);
    static void changeLoggingDestinationTo(std::string const& filename);
    static void log(const enum LogLevel& logLevel, const std::string& message);
 private:
    Logger();
    static enum LogLevel maxLogLevel;
    static std::ofstream outputFile;
};

#endif  // LOGGER_H_
