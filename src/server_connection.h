/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_connection.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/16 14:05:04 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/21 18:18:33 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONNECTION
#define SERVER_CONNECTION

#include <string>

struct ServerConnection {
    ServerConnection();
    ServerConnection(const std::string& configuration);
    void connectToServer(int* fd);
    std::string hostName;
    std::string password;
    uint16_t port;
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
};

#endif  // SERVER_CONNECTION
