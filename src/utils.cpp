#include "utils.h"

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
