#pragma once

#include<d3d9.h> 
#include<d3dx9.h>
#include<vector>
#include "poly2tri/poly2tri.h"
#include "Point2.h"
#include "Edge.h"
#include "Triangle.h"
#include "DxStructs.h"

typedef std::vector<Point2>	obstacle;

class NavMesh
{
    p2t::CDT*                   CTD;
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
    DWORD					    current_color;
    int						    nb_triangles;
    int						    nb_points;

public:
    NavMesh();
    NavMesh(std::vector<Point2> point_list);
    ~NavMesh();
    void set_points(std::vector<Point2> obstacles);
    void add_obstacle(std::vector<obstacle> vListPoints);
    void update(LPDIRECT3DDEVICE9 p_dx_Device);
    void draw(LPDIRECT3DDEVICE9 p_dx_Device);
    void triangulate() throw();
    bool in_circle(Point2 p, Point2 p1, Point2 p2, Point2 p3);
    int  is_inside_polygon(Point2 center);
    std::vector<Triangle*> get_mesh();
};