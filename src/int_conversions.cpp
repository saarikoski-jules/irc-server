/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   int_conversions.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/19 10:33:43 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/19 11:01:33 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "int_conversions.h"

#include <string>

std::string IntConversion::intToString(int value) {
	std::string str;

	if (value == 0) {
		str.push_back('0');
		return (str);
	}
	if (value < 0) {
		str.push_back('-');
		value *= -1;
	}
	while (value != 0) {
		str.push_back(value % 10 + '0');
		value /= 10;
	}
	return (str);
}
