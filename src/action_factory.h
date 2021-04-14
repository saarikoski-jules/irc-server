/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   action_factory.h                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:23 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/14 18:50:10 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_FACTORY_H_
#define ACTION_FACTORY_H_

#include <string>
#include <vector>

#include "iserver_action.h"
#include "server_action.h"

typedef struct actionFormat_s actionFormat_t;

class actionFactory {
 private:
    IServerAction* accept(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    IServerAction* receive(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    IServerAction* disconnect(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    IServerAction* nick(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    IServerAction* user(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    IServerAction* join(
        std::vector<std::string> params, const int& clientFd, Client* cli, const std::string& prefix = "");
    static const actionFormat_t actionFormats[];
    static const size_t actionFormatLen;
 public:
    IServerAction* newAction(
        std::string cmd, std::vector<std::string> params,
        const int& clientFd, Client* cli, const std::string& prefix = "");
};

typedef struct  actionFormat_s {
    IServerAction* (actionFactory::*action)(
        std::vector<std::string>, const int& clientFd, Client* cli, const std::string& prefix);
    std::string type;
}               actionFormat_t;

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
