/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_connection.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 14:05:04 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/28 16:44:52 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONNECTION
#define SERVER_CONNECTION

#define SERVER_CONNECTION_PASSWORD "cats"

#include <string>

struct ServerConnection {
    ServerConnection();
    ServerConnection(const std::string& configuration);
    void connectToServer(int* fd);
    std::string name;
    std::string hopcount;
    std::string token;
    std::string info;
    std::string hostName;
    std::string password;
    uint16_t port;
    std::string version;
    std::string implementation;
    std::string flags;
    std::string options;
};

class ServerConnectionException : public std::exception {
 public:
    ServerConnectionException(std::string const& what, const bool& fatal);
    ~ServerConnectionException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
    std::string fullMessage;
};

#endif  // SERVER_CONNECTION
