/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.cpp                                         :+:    :+:             */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:10 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/25 16:25:07 by jules        ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

#include <string>
#include <vector>

void    *Utils::Mem::set(void *ptr, int c, size_t n) {
    unsigned char *ptr_copy;

    ptr_copy = static_cast<unsigned char *>(ptr);
    while (n > 0) {
        *ptr_copy = c;
        ptr_copy++;
        n--;
    }
    return (static_cast<void *>(ptr));
}

std::vector<std::string> Utils::String::tokenize(
    const std::string& orig, const std::string::size_type end, const std::string& delim) {
    std::vector<std::string> vec;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while (pos < end) {
        pos = orig.find(delim, prev);
        std::string sub;
        if (pos == std::string::npos) {
            sub = orig.substr(prev, orig.length());
        } else {
            sub = orig.substr(prev, pos - prev);
        }
        vec.push_back(sub);
        prev = pos + delim.length();
    }
	for (std::vector<std::string>::iterator i = vec.begin(); i != vec.end();) {
		if (i->length() == 0) {
			i = vec.erase(i);
		} else {
			i++;
		}
	}
	return (vec);
}

bool Utils::String::isAlnum(const std::string& str) {
    for (std::string::const_iterator i = str.begin(); i != str.end(); i++) {
        if ((*i > '9' || *i < '0')
        && (*i > 'z' || *i < 'a')
        && (*i > 'Z' || *i < 'A')) {
            return (false);
        }
    }
    return (true);
}
