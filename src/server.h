/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:00:11 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/13 17:05:33 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <vector>
#include <queue>
#include <cinttypes>
#include <exception>
#include <map>

#include "socket.h"
#include "client.h"
#include "message_parser.h"
#include "server_action.h"
#include "channel.h"

class Server {
 public:
    Server(const uint16_t& port, const std::string& password);
    ~Server();
    void run();
    void sendReplyToClient(const int& clientFd, const std::string& message);
    void acceptNewClient(const int& clientFd);
    void deleteClient(const int& clientFd);
    Client* getClientByFd(const int& clientFd);
    bool nicknameExists(const std::string& nickName);
    bool usernameExists(const std::string& userName);
    void addNewAction(IServerAction* action);
    void createNewChannel(const std::string& name, const int& clientFd);
    Channel* findChannel(const std::string& name);
 protected:
    std::vector<Client> clients;
 private:
    Server();
    std::queue<IServerAction*> actions;
    std::map<std::string, Channel> channels;
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
