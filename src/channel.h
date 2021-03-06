/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   channel.h                                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/20 14:18:51 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/31 16:36:38 by jules        ########   odam.nl          */
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
    void removeMode(char c);
    void addMode(char c);
    std::string getModes() const;
    std::string getModeParams() const;
    void setLimit(unsigned int lim);
	unsigned int getLimit() const;
    void changeKey(const std::string& key);
	std::string getKey() const;
	void removeBanMask(const std::string& mask);
    void addBanMask(const std::string& mask);
    std::string getBanMask(size_t index) const;
	std::vector<Connection*> getConnections() const;
   std::string getNames(Connection* connection) const;
   bool isOper(const Connection* connection) const;
   void removeConnection(const Connection* toRemove);
   bool connectionIsInChannel(const Connection* connection) const;
	size_t getAmtUsers() const;
	std::string getChannelModes() const;

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
