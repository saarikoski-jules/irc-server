/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/07 15:03:38 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_FACTORY_H_
#define ACTION_FACTORY_H_

#include <string>
#include <vector>

#include "iserver_action.h"
#include "server_action.h"

class actionFactory {
 private:
 public:
    IServerAction* accept(std::vector<std::string> params, const int& clientFd);
    IServerAction* receive(std::vector<std::string> params, const int& clientFd);
    IServerAction* disconnect(std::vector<std::string> params, const int& clientFd);
    IServerAction* nick(std::vector<std::string> params, const int& clientFd);
    IServerAction* user(std::vector<std::string> params, const int& clientFd);
    IServerAction* newAction(std::string cmd, std::vector<std::string> params, const int& clientFd);
};

typedef struct actionFormat_s {
    IServerAction* (actionFactory::*action)(std::vector<std::string>, const int& clientFd);
    std::string type;
    int requiredAmtParams;
    int maxAmtParams;
}               actionFormat_t;

static const size_t actionFormatLen = 5;
static actionFormat_t const actionFormats[] = {
    {&actionFactory::accept, "ACCEPT", 0, 0},
    {&actionFactory::receive, "RECEIVE", 1, 1},
    {&actionFactory::disconnect, "DISCONNECT", 0, 0},
    {&actionFactory::nick, "NICK", 1, 2},
    {&actionFactory::user, "USER", 4, 4},
};

class ActionFactoryException: public std::exception {
 public:
    ActionFactoryException(const std::string& what, const bool& fatal);
    ~ActionFactoryException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
};

#endif  // ACTION_FACTORY_H_
