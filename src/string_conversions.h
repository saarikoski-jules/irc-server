/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_conversions.h                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvisser <jvisser@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 14:00:45 by jvisser       #+#    #+#                 */
/*   Updated: 2021/03/31 15:06:46 by jvisser       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_CONVERSIONS_H_
#define STRING_CONVERSIONS_H_

#include <string>
#include <cinttypes>

namespace StringConversion {
    uint16_t toUint16(const std::string& value);
}

#endif  // STRING_CONVERSIONS_H_
