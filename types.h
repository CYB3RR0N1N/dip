#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

struct Point2f
{
    float x;
    float y;
};

struct Pontf3f : public Point2f
{
    float z;
};

struct Point2i
{
    int x;
    int y;
};

struct Point3i : public Point2i
{
    int z;
};


struct Color3
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#endif