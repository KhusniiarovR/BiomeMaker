#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <cmath>

// todo perlinnoise class and better algorithm

inline int Hash(int x, int y) {
    int h = x * 374761393 + y * 668265263;
    h = (h ^ (h >> 13)) * 1274126177;
    return h;
}

inline float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

inline float Smoothstep(float t) {
    return t * t * (3 - 2 * t);
}

inline float Random(int x, int y) {
    return (Hash(x, y) & 0x7fffffff) / (float)0x7fffffff;
}

inline float PerlinNoise(float x, float y) {
    int x0 = (int)floor(x);
    int y0 = (int)floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = Smoothstep(x - x0);
    float sy = Smoothstep(y - y0);

    float n0 = Random(x0, y0);
    float n1 = Random(x1, y0);
    float ix0 = Lerp(n0, n1, sx);

    n0 = Random(x0, y1);
    n1 = Random(x1, y1);
    float ix1 = Lerp(n0, n1, sx);

    return Lerp(ix0, ix1, sy);
}

#endif //PERLINNOISE_H