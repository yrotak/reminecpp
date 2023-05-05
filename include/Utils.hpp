#ifndef UTILS_HPP
#define UTILS_HPP
#include <vector>

using Uuid = std::vector<unsigned char>;

template<class T>
void endswap(T *objp) {
    unsigned char *memp = reinterpret_cast<unsigned char *>(objp);
    std::reverse(memp, memp + sizeof(T));
}

template<class T>
void endswap(T &obj) {
    unsigned char *raw = reinterpret_cast<unsigned char *>(&obj);
    std::reverse(raw, raw + sizeof(T));
}
inline void endswap(unsigned char *arr, size_t arrLen) {
    std::reverse(arr, arr + arrLen);
}
#endif