#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <string>
#include <vector>

#include "client.h"
#include "reply.h"

class Channel {
 public:
    Channel(const std::string& name);
    const std::string name;
    void addClient(Client* client, const std::string& key);
 private:
    std::vector<Client *> clients;
    std::string key;
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
