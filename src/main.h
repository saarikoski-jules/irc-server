/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:13:44 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 13:15:02 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <exception>

class ArgumentException : public std::exception {
 public:
    ArgumentException(std::string const& what, const bool& fatal);
    ~ArgumentException() throw();
    const bool& isFatal() const;
    const char* what() const throw();
 private:
    const bool fatal;
    const std::string message;
};

#endif  // MAIN_H_
