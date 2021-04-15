/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_conversions.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:39:29 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/15 14:04:43 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "string_conversions.h"

#include <string>
#include <sstream>
#include <climits>
#include <cinttypes>

unsigned int StringConversion::toUint(const std::string& value) {
    unsigned long num;
    std::stringstream ss;

    ss << value;
    ss >> num;
    if ((num == 0 && value != "0")
    || (num > UINT_MAX || num < 0)) {
        throw std::out_of_range("Cannot convert string to uint");
    }
    return (static_cast<unsigned int>(num));
}

uint16_t StringConversion::toUint16(const std::string& value) {
    unsigned int num;

    num = StringConversion::toUint(value);
    if (num > USHRT_MAX) {
        throw std::out_of_range("Cannot convert string to uint16");
    }
    return (static_cast<uint16_t>(num));
}
