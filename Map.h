#pragma once

#include <windowsx.h>
#include <vector>
#include "DxStructs.h"

class Map
{
    std::vector<Point2>	points_list;
    bool			    complete_path;
    bool			    internal_complete_path;
    Point2			    current_point;
    bool			    follow_point_added;
    int				    index_follow_point;
public:
    Map();
    ~Map();
    void WindowProcedure(UINT msg, WPARAM wParam, LPARAM lParam);
    float distance(Point2 p1, Point2 p2);
    bool complete();
    void set_complete(bool complete);
    std::vector<Point2> get_points_list();
    void set_points_list(std::vector<Point2> points_list);
    std::vector<obstacle> get_obstacles(int demo_map);
    void set_demo_map(int demo_map);
    std::vector<ITEM> get_obstacles_types(int demo_map);
    int change_x(int val);
    int change_y(int val);
};


//0 0 0 1 2 0 3 2

//140 88
//70 88
//64 64
//32 32 