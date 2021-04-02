/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 10:00:11 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/01 10:44:30 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <cinttypes>
#include <exception>

#include "socket.h"

class Server {
 public:
    Server(const uint16_t port, std::string const& password);
    ~Server();
 private:
    Server();
    void validatePassword(std::string const& password) const;
    void openSocket(const int& port);
    void listenOnSocket();
    Socket socket;
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
