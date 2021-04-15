/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_conversions.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:00:45 by jvisser       #+#    #+#                 */
/*   Updated: 2021/04/15 13:58:57 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_CONVERSIONS_H_
#define STRING_CONVERSIONS_H_

#include <string>
#include <cinttypes>

namespace StringConversion {
    uint16_t toUint16(const std::string& value);
    unsigned int toUint(const std::string& value);
}

#endif  // STRING_CONVERSIONS_H_
