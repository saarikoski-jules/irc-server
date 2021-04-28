/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 13:02:31 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/28 17:48:41 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "action_factory.h"

#include <vector>

#include "server_action_disconnect.h"
#include "server_action_accept.h"
#include "server_action_receive.h"
#include "server_action_nick.h"
#include "server_action_user.h"
#include "server_action_join.h"
#include "server_action_mode.h"
#include "server_action_send.h"
#include "server_action_ping.h"

const size_t actionFactory::actionFormatLen = 9;

const actionFormat_t actionFactory::actionFormats[] = {
    {&actionFactory::accept, "ACCEPT"},
    {&actionFactory::receive, "RECEIVE"},
    {&actionFactory::disconnect, "DISCONNECT"},
    {&actionFactory::nick, "NICK"},
    {&actionFactory::user, "USER"},
    {&actionFactory::join, "JOIN"},
    {&actionFactory::mode, "MODE"},
    {&actionFactory::send, "SEND"},
    {&actionFactory::ping, "PING"},
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

IServerAction* actionFactory::mode(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionMode(params, fd, prefix));
}

IServerAction* actionFactory::send(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionSend(params, fd, prefix));
}

IServerAction* actionFactory::ping(
    std::vector<std::string> params, const int& fd, const std::string& prefix) {
    return (new ServerActionPing(params, fd, prefix));
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
