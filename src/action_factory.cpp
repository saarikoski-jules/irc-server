/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 13:02:31 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 17:46:04 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "action_factory.h"

//TODO(Jules): send numeric reply when needed
IServerAction* actionFactory::accept(std::vector<std::string> params, const int& clientFd) {
    return (new ServerActionAccept(params, clientFd));
}

IServerAction* actionFactory::receive(std::vector<std::string> params, const int& clientFd) {
    return (new ServerActionReceive(params, clientFd));
}

IServerAction* actionFactory::disconnect(std::vector<std::string> params, const int& clientFd) {
    return (new ServerActionDisconnect(params, clientFd));
}

IServerAction* actionFactory::nick(std::vector<std::string> params, const int& clientFd) {
    return (new ServerActionNick(params, clientFd));
}

IServerAction* actionFactory::newAction(std::string cmd, std::vector<std::string> params, const int& clientFd) {
    for (unsigned int i = 0; i < actionFormatLen; i++) {
        if (actionFormats[i].type == cmd) {
            if (params.size() >= (const size_t)actionFormats[i].requiredAmtParams && params.size() <= (const size_t)actionFormats[i].maxAmtParams) {
                IServerAction* (actionFactory::*ac)(std::vector<std::string>, const int&) = actionFormats[i].action;
                return (this->*ac)(params, clientFd);
            } else {
                throw ActionFactoryException("command has a bad amount of params", false);
            }
        }
    }
    throw ActionFactoryException("invalid action", false);
}

ActionFactoryException::ActionFactoryException(const std::string& message, const bool& fatal) :
fatal(fatal),
message(message) {
}

ActionFactoryException::~ActionFactoryException() throw() {
}

const bool& ActionFactoryException::isFatal() const {
    return (fatal);
}

const char* ActionFactoryException::what() const throw() {
    if (isFatal()) {
        return (std::string("Fatal action factory exception: " + message).c_str());
    }
    return (std::string("Action factory exception: " + message).c_str());
}
