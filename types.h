#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

struct Point2i;

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
    Point2i(){}
    Point2i(int _x, int _y) { x = _x ; y = _y; }
    int x;
    int y;
    Point2i operator+(Point2i a)
    {
        return {this->x + a.x, this->y + a.y};
    }

    Point2i operator-(Point2i a)
    {
        return {this->x - a.x, this->y - a.y};
    }
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