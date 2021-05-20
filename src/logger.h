/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Logger.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/28 17:23:48 by jvisser       #+#    #+#                 */
/*   Updated: 2021/05/19 12:42:28 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

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
    static void log(const enum LogLevel& logLevel, const std::string& message);
 private:
    Logger();
    static enum LogLevel maxLogLevel;
};

#endif  // LOGGER_H_
