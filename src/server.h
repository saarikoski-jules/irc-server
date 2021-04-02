/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:00:11 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/02 16:59:22 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <vector>
#include <cinttypes>
#include <exception>

#include "server_action.h"
#include "socket.h"
#include "client.h"

class Server {
 public:
    Server(const uint16_t port, std::string const& password);
    ~Server();
    void run();
 private:
    Server();
    ServerAction action;
    Socket socket;
    std::vector<Client> clients;
    void validatePassword(std::string const& password) const;
    void openSocket(const uint16_t& port);
    void listenOnSocket();
    void handleAction();
    void acceptNewClient();
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
