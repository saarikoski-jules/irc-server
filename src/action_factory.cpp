/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 13:02:31 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/20 12:12:04 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "action_factory.h"
#include "server_action_disconnect.h"
#include "server_action_accept.h"
#include "server_action_receive.h"
#include "server_action_nick.h"
#include "server_action_user.h"
#include "server_action_join.h"
#include "server_action_mode.h"

#include <vector>

const size_t actionFactory::actionFormatLen = 7;

const actionFormat_t actionFactory::actionFormats[] = {
    {&actionFactory::accept, "ACCEPT"},
    {&actionFactory::receive, "RECEIVE"},
    {&actionFactory::disconnect, "DISCONNECT"},
    {&actionFactory::nick, "NICK"},
    {&actionFactory::user, "USER"},
    {&actionFactory::join, "JOIN"},
    {&actionFactory::mode, "MODE"},
};

// TODO(Jules): send numeric reply when needed
IServerAction* actionFactory::accept(
    std::vector<std::string> params, const int& clientFd, Client*, const std::string& prefix) {
    return (new ServerActionAccept(params, clientFd, NULL, prefix));
}

IServerAction* actionFactory::receive(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) {
    return (new ServerActionReceive(params, clientFd, cli, prefix));
}

IServerAction* actionFactory::disconnect(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) {
    return (new ServerActionDisconnect(params, clientFd, cli, prefix));
}

IServerAction* actionFactory::nick(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) {
    return (new ServerActionNick(params, clientFd, cli, prefix));
}

IServerAction* actionFactory::user(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) {
    return (new ServerActionUser(params, clientFd, cli, prefix));
}

IServerAction* actionFactory::join(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) {
    return (new ServerActionJoin(params, clientFd, cli, prefix));
}

IServerAction* actionFactory::mode(
    std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix) {
    return (new ServerActionMode(params, clientFd, cli, prefix));
}

IServerAction* actionFactory::newAction(
    std::string cmd, std::vector<std::string> params,
    const int& clientFd, Client* cli, const std::string& prefix) {
    for (unsigned int i = 0; i < actionFormatLen; i++) {
        if (actionFormats[i].type == cmd) {
            return (this->*actionFormats[i].action)(params, clientFd, cli, prefix);
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
