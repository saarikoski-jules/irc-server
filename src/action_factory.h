/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/06 14:39:48 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTIONFACTORY_H_
#define ACTIONFACTORY_H_

#include <string>

#include "IServerAction.h"
#include "server_action.h"

class actionFactory {
 private:
 public:
    IServerAction* nick(std::vector<std::string> params, const std::vector<Client>& clients);
    IServerAction* newAction(std::string cmd, std::vector<std::string> params, const std::vector<Client>& clients);
};

typedef struct actionFormat_s {
    IServerAction* (actionFactory::*action)(std::vector<std::string>, const std::vector<Client>&);
    std::string type;
    int requiredAmtParams;
    int maxAmtParams;
}               actionFormat_t;

// const actionFormatLen = 6;
const size_t actionFormatLen = 1;

static actionFormat_t const actionFormats[] = {
    // {actionFactory::noAction, "NO_ACTION", 0, 0},
    // {"ACCEPT", 0, 0},
    // {"RECEIVE", 1, 1},
    // {"DISCONNECT", 1, 1},
    {&actionFactory::nick, "NICK", 1, 2},
    // {"USER", 4, 4}
};

enum actionType {
    NO_ACTION,
    ACCEPT,
    RECEIVE,
    DISCONNECT,
    NICK,
    USER
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

#endif  // ACTIONFACTORY_H_
