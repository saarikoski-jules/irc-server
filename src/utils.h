/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsaariko <jsaariko@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/04/08 13:31:08 by jsaariko      #+#    #+#                 */
/*   Updated: 2021/05/03 13:15:21 by jsaariko      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <string>
#include <vector>

namespace Utils {
template<typename T>
T clamp(T value, T min, T max) {
    if (value > max) {
        return (max);
    } else if (value < min) {
        return (min);
    }
    return (value);
}

namespace Mem {
    void    *set(void *ptr, int c, size_t n);
}
namespace String {
    std::vector<std::string> tokenize(
        const std::string& orig, const std::string::size_type end, const std::string& delim);
    bool isAlnum(const std::string& str);
}
}

#endif  // UTILS_H_
