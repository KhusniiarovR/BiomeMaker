#ifndef CLAMP_H
#define CLAMP_H

template <typename T>
T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

#endif //CLAMP_H
