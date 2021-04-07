#ifndef UTILS_H_
#define UTILS_H_

#include <stddef.h>
#include <string>
#include <vector>

namespace Utils {
namespace Mem {
    void    *set(void *ptr, int c, size_t n);
}
namespace String {
    std::vector<std::string> tokenize(const std::string& orig, const char* delim);
}
}

#endif  // UTILS_H_
