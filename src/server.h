/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:00:11 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/07 15:56:27 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <vector>
#include <queue>
#include <cinttypes>
#include <exception>

#include "server_action.h"
#include "socket.h"
#include "client.h"
#include "message_parser.h"

class Server {
 public:
    Server(const uint16_t& port, const std::string& password);
    ~Server();
    void run();
    void acceptNewClient(const int& clientFd);
    void deleteClient(const int& clientFd);
    Client* getClientByFd(const int& clientFd);
    bool nicknameExists(const std::string& nickName);
    bool usernameExists(const std::string& userName);
    void addNewAction(IServerAction* action);
 protected:
    std::vector<Client> clients;
 private:
    Server();
    std::queue<IServerAction*> actions;
    Socket socket;
    MessageParser parser;
    void validatePassword(std::string const& password) const;
    void openSocket(const uint16_t& port);
    void listenOnSocket();
    void handleAction();
};

class ServerException : public std::exception {
 public:
    ServerException(std::string const& what, const bool& fatal);
    ~ServerException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
};

#endif  // SERVER_H_
