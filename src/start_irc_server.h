/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   start_irc_server.h                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 15:38:06 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/23 12:30:21 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef START_IRC_SERVER_H_
#define START_IRC_SERVER_H_

#include <string>
#include <exception>

void startIrcServerFromArguments(const int argc, char* const* argv);

class ArgumentException : public std::exception {
 public:
    ArgumentException(std::string const& what, const bool& fatal);
    ~ArgumentException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
    std::string fullMessage;
};

#endif  // START_IRC_SERVER_H_
