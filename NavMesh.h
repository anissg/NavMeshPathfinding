#pragma once

#include<d3d9.h> 
#include<d3dx9.h>
#include<vector>
#include "poly2tri/poly2tri.h"
#include "Point2.h"
#include "Edge.h"
#include "Triangle.h"
#include "DxStructs.h"

class NavMesh
{
    p2t::CDT*                   CTD; // Delauney triangulation
    std::vector<obstacle>		obstacles;
    std::vector<Point2>			points_list;
    std::vector<p2t::Point*>	points_list_2;
    std::vector<Triangle>		triangles_list;
    std::vector<p2t::Triangle*>	triangles_list_2;
    std::vector<Edge>			edges_list;
    IDirect3DVertexBuffer9*     vertex_buffer;
    IDirect3DVertexBuffer9*     vertex_buffer_triangles;
    CUSTOMVERTEX			    square_vertices[6];
    D3DMATERIAL9			    material;
    DWORD					    color;
    int						    nb_triangles;
    int						    nb_points;

public:
    NavMesh();
    NavMesh(std::vector<Point2> point_list);
    ~NavMesh();
    void set_points(std::vector<Point2> points);
    void add_obstacle(std::vector<obstacle> obstacles);
    void update(LPDIRECT3DDEVICE9 dx_device);
    void draw(LPDIRECT3DDEVICE9 dx_device);
    int  is_inside_polygon(Point2 center);
    std::vector<p2t::Triangle*> get_mesh();
};