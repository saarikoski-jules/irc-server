/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   str_to_short.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:39:29 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/03/31 13:48:16 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include <string>

uint16_t str_to_short(std::string value) {
    uint16_t num;
    std::stringstream ss;

    ss << value;
    ss >> num;
    if (num == 0 && value != "0") {
        throw std::exception();
    }
    return (static_cast<uint16_t>(num));
}
