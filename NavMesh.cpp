#include "NavMesh.h"

NavMesh::NavMesh()
{
}

NavMesh::NavMesh(std::vector<Point2> point_list)
{
}

NavMesh::~NavMesh()
{
}

void NavMesh::set_points(std::vector<Point2> obstacles)
{
}

void NavMesh::add_obstacle(std::vector<obstacle> vListPoints)
{
}

void NavMesh::update(LPDIRECT3DDEVICE9 p_dx_Device)
{
}

void NavMesh::draw(LPDIRECT3DDEVICE9 p_dx_Device)
{
}

void NavMesh::triangulate() throw()
{
}

bool NavMesh::in_circle(Point2 p, Point2 p1, Point2 p2, Point2 p3)
{
    return false;
}

int NavMesh::is_inside_polygon(Point2 center)
{
    return 0;
}

std::vector<Triangle*> NavMesh::get_mesh()
{
    return std::vector<Triangle*>();
}
