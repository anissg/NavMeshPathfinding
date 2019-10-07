#pragma once

class Point2
{
public:
    int X;
    int Y;

    Point2() : X(0), Y(0)
    {
    }

    Point2(int x, int y) : X(x), Y(y)
    {
    }

    Point2(float x, float y) : X(x), Y(y)
    {
    }

    bool operator == (const Point2& p)
    {
        return X == p.X && Y == p.Y;
    }

    bool operator != (const Point2& p)
    {
        return X != p.X || Y != p.Y;
    }
};