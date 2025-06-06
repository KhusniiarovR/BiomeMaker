#ifndef PAIRHASH_H
#define PAIRHASH_H

#include <functional>

struct PairHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second << 1);
    }
};

#endif //PAIRHASH_H
