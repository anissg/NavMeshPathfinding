#pragma once

class Point2
{
public:
    int x;
    int y;

    Point2() : x(0), y(0)
    {
    }

    Point2(int x, int y) : x(x), y(y)
    {
    }

    Point2(float x, float y) : x(x), y(y)
    {
    }

    bool operator == (const Point2& p)
    {
        return x == p.x && y == p.y;
    }

    bool operator != (const Point2& p)
    {
        return x != p.x || y != p.y;
    }
};