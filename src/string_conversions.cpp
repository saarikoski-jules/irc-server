/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_conversions.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:39:29 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/03/31 14:42:37 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "string_conversions.h"

uint16_t StringConversion::toUint16(const std::string& value) {
    int num;
    std::stringstream ss;

    ss << value;
    ss >> num;
    if ((num == 0 && value != "0")
    || (num > USHRT_MAX || num < 0)) {
        throw std::out_of_range("Cannot convert string to uint16");
    }
    return (static_cast<uint16_t>(num));
}
