/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 13:02:31 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 15:17:59 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "action_factory.h"

IServerAction* actionFactory::nick(std::vector<std::string> params, const std::vector<Client>& clients) {
    IServerAction* act = new ServerActionNick(clients, params);
    //TODO(Jules): send numeric reply when needed
    return (act);
}


#include <iostream>
IServerAction* actionFactory::newAction(std::string cmd, std::vector<std::string> params, const std::vector<Client>& clients) {
    for (unsigned int i = 0; i < actionFormatLen; i++) {
        std::cout << actionFormats[i].type << " == " << cmd << std::endl;
        if (actionFormats[i].type == cmd) {
            if (params.size() >= (const size_t)actionFormats[i].requiredAmtParams && params.size() <= (const size_t)actionFormats[i].maxAmtParams) {
                IServerAction* (actionFactory::*ac)(std::vector<std::string>, const std::vector<Client>&) = actionFormats[i].action;
                return (this->*ac)(params, clients);
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
