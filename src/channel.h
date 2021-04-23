/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   channel.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/23 12:35:51 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 12:35:52 by jvisser       ########   odam.nl         */
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
 private:
    Connection* chanop;
    std::vector<Connection *> connections;
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
    std::string fullMessage;
};

#endif  // CHANNEL_H_
