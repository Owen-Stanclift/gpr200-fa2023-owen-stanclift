////Glenn Sakowicz
#include "Perlin.h"

#include <algorithm>

PerlinNoise::PerlinNoise() {
    // Initialize permutation
    for (int i = 0; i < 512; ++i) {
        perm[i] = i < 256 ? perm[i] = perm[i + 256] = perm[i] : rand() % 256;
    }
}

float PerlinNoise::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float PerlinNoise::gradient(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::noise(float x, float y) const {
    int X = static_cast<int>(floor(x)) & 255;
    int Y = static_cast<int>(floor(y)) & 255;
    x -= floor(x);
    y -= floor(y);
    float u = fade(x);
    float v = fade(y);
    int A = perm[X] + Y, AA = perm[A], AB = perm[A + 1];
    int B = perm[X + 1] + Y, BA = perm[B], BB = perm[B + 1];
    return lerp(v, lerp(u, gradient(perm[AA], x, y), gradient(perm[BA], x - 1, y)),
        lerp(u, gradient(perm[AB], x, y - 1), gradient(perm[BB], x - 1, y - 1))) * 2;
}