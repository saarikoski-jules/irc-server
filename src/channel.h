/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   channel.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 14:18:51 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/18 14:31:57 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>
#include <vector>

#include "connection.h"
#include "client.h"
#include "reply.h"

class Channel {
 public:
    Channel(const std::string& name, Connection* chanop);
    const std::string name;
    void addClient(Connection* client, const std::string& key);
    bool topicIsSet;
    std::string topic;
    void addOperator(Connection* newChanop);
    void removeOperator(Connection* newChanop);
    bool isOperator(Connection* cli) const;
    void removeMode(char c);
    void addMode(char c);
    std::string getModes() const;
    std::string getModeParams() const;
    void setLimit(unsigned int lim);
    void changeKey(const std::string& key);
    void removeBanMask(const std::string& mask);
    void addBanMask(const std::string& mask);
    std::string getBanMask(size_t index) const;
	std::vector<Connection*> getConnections(const Connection& client) const;
   std::string getNames(Connection* connection) const;
   bool isOper(Connection* connection) const;
   void removeConnection(Connection* toRemove);

 private:
    //std::vector<Client*> chanops;
    //std::vector<Client*> clients;
    std::vector<std::string> bans;
    std::vector<Connection*> chanops;
    //Connection* chanop;
    void clientHasAccess(Connection* connection) const;
    std::vector<Connection *> connections;
    std::string key;
    std::string modes;
    unsigned int limit;
    bool canJoin(Client* client, const std::string& key) const;
    bool isBanned(Client* client) const;
    // v, ability to speak on moderated channel
};

class ChannelException : public std::exception {
 public:
    ChannelException(const std::string& what, const bool& fatal);
    ~ChannelException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
    std::string fullMessage;
};

#endif  // CHANNEL_H_
