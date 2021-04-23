/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   start_irc_server.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:37:35 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/21 18:57:02 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "start_irc_server.h"

#include <string>
#include <exception>

#include "server.h"
#include "logger.h"
#include "connection.h"
#include "string_conversions.h"

static void startNewIrcServerFromArguments(char* const* argv) {
    try {
        Server server(StringConversion::toUint16(argv[1]), argv[2]);
        server.run();
    } catch (const std::out_of_range& e) {
        throw ArgumentException("Port number is not in range of valid ports", true);
    }
}

static void startConnectingIrcServerFromArguments(char* const* argv) {
    try {
        Connection startingServer(argv[1]);
        Server server(&startingServer, StringConversion::toUint16(argv[2]), argv[3]);
        server.run();
    } catch (const ServerConnectionException& e) {
        Logger::log(LogLevelFatal, e.what());
        throw ArgumentException("Invalid first argument", true);
    }
}

void startIrcServerFromArguments(const int argc, char* const* argv) {
    if (argc == 3) {
        Logger::log(LogLevelDebug, "Attempting to start new irc server");
        startNewIrcServerFromArguments(argv);
    } else if (argc == 4) {
        Logger::log(LogLevelDebug, "Attempting to connect to existing irc server");
        startConnectingIrcServerFromArguments(argv);
    } else {
        throw ArgumentException("Invalid amount of arguments provided", true);
    }
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
