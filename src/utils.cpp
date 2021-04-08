/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/06 11:43:10 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/04/08 15:49:39 by jsaariko      ########   odam.nl         */
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

std::vector<std::string> Utils::String::tokenize(const std::string& orig, const std::string& delim) {
    std::vector<std::string> vec;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while (pos != std::string::npos) {
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
    return (vec);
}
