/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:00:11 by jvisser       #+#    #+#                 */
/*   Updated: 2021/06/09 13:07:47 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H_
#define SERVER_H_

#define SERVERTOKEN "4242"

#include <string>
#include <vector>
#include <queue>
#include <cinttypes>
#include <exception>
#include <map>
#include <utility>

#include "socket.h"
#include "connection.h"
#include "message_parser.h"
#include "iserver_action.h"
#include "channel.h"

class Server {
 public:
 	static std::string serverName;
    Server(const uint16_t& port, const std::string& password);
    Server(Connection* startingServer, const uint16_t& port, const std::string& password);
    ~Server();
    void sendAuthenticationTo(const int& fd, const std::string& password);
    void run();
    void sendMessage(const int& fd, const std::string& message);
    void sendMessageToServer(const int& fd, const std::string& message);
    void sendMessageToAllServers(const std::string& message);
    void sendMessageToAllServersButOne(const std::string& message, const int& exceptionFd);
    void sendReplyToClient(const int& clientFd,const std::string& message, const std::string &prefix = Server::serverName);
    void sendErrorToConnectionBypassingQueue(const int& fd, const std::string& message);
    void burstServerInformationTo(const int& fd);
    Connection* getClientByNick(const std::string& nick);
    void acceptNewConnection(const int& fd);
    void deleteConnection(const int& fd);
    Connection* getConnectionByFd(const int& fd);
    bool hasLocalConnection(const Connection& connection);
    bool fdExists(const int& fd);
    bool nicknameExists(const std::string& nickName);
    bool usernameExists(const std::string& userName);
    bool servernameExists(const std::string& serverName);
    bool serverTokenExists(const std::string& serverToken);
    void addNewAction(IServerAction* action);
    Channel* createNewChannel(const std::string& name, Connection* chanop);
    Channel* findChannel(const std::string& name);
    void removeClientFromChannels(const Connection* con);
    void sendMessageToAllLocalUsersInClientChannels(const Connection* connection, const std::string& message, const std::string& prefix);
	void deleteChannel(Channel* chan);
    void delayFirstAction();
	time_t serverStart;
	std::map<std::string, Channel*> getListOfChannels();
 protected:
    std::map<const int, Connection*> connections;
 private:
    Server();
    std::queue<IServerAction*> actions;
    std::queue<IServerAction*> delayedActions;
    std::map<std::string, Channel*> channels;
    Socket serverSocket;
    MessageParser parser;
    void validatePassword(std::string const& password) const;
    void openSocket(const uint16_t& port);
    void listenOnSocket();
    void handleAction();
    void burstConnections(const int& fd);
    void burstLeafConnections(Connection& connection, const int& fd);
    void burstChannels(const int& fd);
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
    std::string fullMessage;
};

#endif  // SERVER_H_
