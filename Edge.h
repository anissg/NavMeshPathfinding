#pragma once

class Edge
{
public:
    int p1;
    int p2;
    
    Edge(int p1, int p2) : p1(p1), p2(p2)
    {
    }

    bool operator == (const Edge& e)
    {
        return (p1 == e.p1 && p2 == e.p2) || (p1 == e.p2 && p2 == e.p1);
    }
};