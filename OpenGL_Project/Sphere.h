#pragma once
#include <array>

class Sphere
{
public:
    float radius;
    float x, y, z;
    float r, g, b,a;
    const int size = 8 /** sizeof(float)*/;
    std::array<float, 8> getArr()
    {
        return std::array<float, 8>{radius, x, y, z, r, g, b,a};
    }
};
