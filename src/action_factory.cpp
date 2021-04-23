/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 13:02:31 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/23 11:18:54 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "action_factory.h"

#include <vector>

const size_t actionFactory::actionFormatLen = 6;

const actionFormat_t actionFactory::actionFormats[] = {
    {&actionFactory::accept, "ACCEPT"},
    {&actionFactory::receive, "RECEIVE"},
    {&actionFactory::disconnect, "DISCONNECT"},
    {&actionFactory::nick, "NICK"},
    {&actionFactory::user, "USER"},
    {&actionFactory::join, "JOIN"},
};

// TODO(Jules): send numeric reply when needed
IServerAction* actionFactory::accept(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionAccept(params, fd, prefix));
}

IServerAction* actionFactory::receive(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionReceive(params, fd, prefix));
}

IServerAction* actionFactory::disconnect(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionDisconnect(params, fd, prefix));
}

IServerAction* actionFactory::nick(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionNick(params, fd, prefix));
}

IServerAction* actionFactory::user(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionUser(params, fd, prefix));
}

IServerAction* actionFactory::join(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionJoin(params, fd, prefix));
}

IServerAction* actionFactory::newAction(
    std::string cmd, std::vector<std::string> params,
    const int& fd, const std::string& prefix) {
    for (unsigned int i = 0; i < actionFormatLen; i++) {
        if (actionFormats[i].type == cmd) {
            return (this->*actionFormats[i].action)(params, fd, prefix);
        }
    }
    throw ActionFactoryException("invalid action", false);
}

ActionFactoryException::ActionFactoryException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
    if (isFatal()) {
        fullMessage = std::string("Fatal action factory exception: " + message);
    } else {
        fullMessage = std::string("Action factory exception: " + message);
    }
}

ActionFactoryException::~ActionFactoryException() throw() {
}

const bool& ActionFactoryException::isFatal() const {
    return (fatal);
}

const char* ActionFactoryException::what() const throw() {
    return (fullMessage.c_str());
}
