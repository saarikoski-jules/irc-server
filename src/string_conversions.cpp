/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string_conversions.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/03/31 13:39:29 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/19 11:30:00 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "string_conversions.h"

#include <string>
#include <climits>
#include <cinttypes>
#include <cstring>

int	ft_atoi(const char *s)
{
	int					i;
	unsigned long       total;
	int					sign;

	i = 0;
	total = 0;
	sign = 1;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] <= '9' && s[i] >= '0')
	{
		total = 10 * total + (s[i] - '0');
		i++;
	}
	return ((int)(total * sign));
}

unsigned int StringConversion::toUint(const std::string& value) {
    uint64_t num;

    num = ft_atoi(value.c_str());
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
