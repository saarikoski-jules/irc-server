/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 09:04:39 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 15:05:47 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#include <string>
#include <exception>

#include "server.h"
#include "logger.h"
#include "string_conversions.h"

void startNewIrcServerFromArguments(char* const* argv) {
    try {
        Server server(StringConversion::toUint16(argv[1]), argv[2]);
    } catch (const std::out_of_range& e) {
        throw ArgumentException("Port number is not in range of valid ports", true);
    }
}

void startIrcServerFromArguments(const int argc, char* const* argv) {
    if (argc == 3) {
        Logger::log(LogLevelDebug, "Attempting to start new irc server");
        startNewIrcServerFromArguments(argv);
    } else if (argc == 4) {
        Logger::log(LogLevelDebug, "Attempting to connect to existing irc server");
        // TODO(Jelle) Connect server to other server.
    }
    throw ArgumentException("Invalid amount of arguments provided", true);
}

int main(int argc, char** argv) {
    try {
        Logger::changeLoggingDestinationTo("./logs/latest.log");
        Logger::log(LogLevelInfo, "Program started");
        startIrcServerFromArguments(argc, argv);
    } catch (const ArgumentException& e) {
        Logger::log(LogLevelFatal, e.what());
    } catch (const ServerException& e) {
        Logger::log(LogLevelFatal, e.what());
    }
    Logger::log(LogLevelInfo, "Program ended gracefully");
    return (0);
}

ArgumentException::ArgumentException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
}

ArgumentException::~ArgumentException() throw() {
}

const bool& ArgumentException::isFatal() const {
    return (fatal);
}

const char* ArgumentException::what() const throw() {
    if (isFatal()) {
        return (std::string("Fatal argument exception: " + message).c_str());
    }
    return (std::string("Argument exception: " + message).c_str());
}
