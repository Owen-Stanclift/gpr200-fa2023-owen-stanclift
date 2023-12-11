#pragma once

class PerlinNoise {
public:
    PerlinNoise();
    float noise(float x, float y) const;

private:
    static float fade(float t);
    static float lerp(float t, float a, float b);
    static float gradient(int hash, float x, float y);

    unsigned char perm[512];
};