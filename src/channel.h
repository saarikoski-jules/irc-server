#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>
#include <vector>

#include "client.h"
#include "reply.h"

class Channel {
 public:
    Channel(const std::string& name, Client* chanop);
    const std::string name;
    void addClient(Client* client, const std::string& key);
    bool topicIsSet;
    std::string topic;
    void addOperator(Client* newChanop);
    void removeOperator(Client* newChanop);
    bool isOperator(Client* cli) const;
    void removeMode(char c);
    void addMode(char c);
    std::string getModes() const;
    std::string getModeParams() const;
    void setLimit(unsigned int lim);
    void changeKey(const std::string& key);
    void removeBanMask(const std::string& mask);
    void addBanMask(const std::string& mask);
    std::string getBanMask(size_t index) const;
 private:
   //  Client* chanop;//turn into a vector
    std::vector<Client*> chanops;
    std::vector<Client*> clients;
    std::vector<std::string> bans;
    std::string key;
    std::string modes;
    unsigned int limit;
    bool canJoin(Client* client, const std::string& key) const;
    //ban mask
    //v, ability to speak on moderated channel
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
};

#endif  // CHANNEL_H_
