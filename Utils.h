#pragma once
#include "poly2tri/poly2tri.h"

bool is_point_in_triangle(p2t::Triangle t, p2t::Point* p)
{
    p2t::Point* p0 = t.GetPoint(0);
    p2t::Point* p1 = t.GetPoint(1);
    p2t::Point* p2 = t.GetPoint(2);

    float z1 = p0->x * (p1->y - p->y) + p1->x * (p->y - p0->y) + p->x * (p0->y - p1->y);
    float z2 = p1->x * (p2->y - p->y) + p2->x * (p->y - p1->y) + p->x * (p1->y - p2->y);
    float z3 = p2->x * (p0->y - p->y) + p0->x * (p->y - p2->y) + p->x * (p2->y - p0->y);

    return (z1 > 0 && z2 > 0 && z3 > 0) || (z1 < 0 && z2 < 0 && z3 < 0);
}

p2t::Point get_middle_point(p2t::Triangle* t)
{
    p2t::Point* p0 = t->GetPoint(0);
    p2t::Point* p1 = t->GetPoint(1);
    p2t::Point* p2 = t->GetPoint(2);

    return p2t::Point((p0->x + p1->x + p2->x) / 3.0, (p0->y + p1->y + p2->y) / 3.0);
}

float get_area(p2t::Triangle t)
{
    float area = 0.0f;

    p2t::Point* p0 = t.GetPoint(0);
    p2t::Point* p1 = t.GetPoint(1);
    p2t::Point* p2 = t.GetPoint(2);
    
    /*
        A = | Ax (By-Cy) + Bx (Cy - Ay) + Cx(Ay - By) | / 2
    */
    
    area += p0->x + (p1->y - p2->y);
    area += p1->x + (p2->y - p0->y);
    area += p2->x + (p0->y - p1->y);
    area = abs(area / 2.0f);

    return area;
}