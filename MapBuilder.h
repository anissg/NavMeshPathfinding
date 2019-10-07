#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <vector>
#include "poly2tri/poly2tri.h"
#include "DxStructs.h"
#include "NavMesh.h"

class MapBuilder
{
    int					    mouse_state;
    int					    selected_items;
    std::vector<ITEM>		elements;
    std::vector<obstacle>	obstacles;
    p2t::Point*             start_point;
    p2t::Point*             end_point;

    bool				    visible;
    bool				    reset;
    int					    interface_state;
    int					    selected_algorithm;
    int					    selected_map;

public:
    MapBuilder();
    ~MapBuilder();
    
    void draw(LPDIRECT3DDEVICE9 p_dx_Device);

    void WindowProcedure(UINT uint_Message, WPARAM parameter1, LPARAM parameter2);
    void MouseEventNavMesh(UINT uint_Message, WPARAM parameter1, LPARAM parameter2);
    void MouseEventPutItems(UINT uint_Message, WPARAM parameter1, LPARAM parameter2);
    void MouseEventPutStartPoint(UINT uint_Message, WPARAM parameter1, LPARAM parameter2);
    void MouseEventPutEndPoint(UINT uint_Message, WPARAM parameter1, LPARAM parameter2);

    std::vector<obstacle> get_obstacles();
    void set_obstacles(std::vector<obstacle> obstacles);
    void set_obstacles_types(std::vector<ITEM> elements);
    void set_interfae_state(int state);
    void set_reset(bool reset);
    bool get_reset();
    int get_state();
    p2t::Point* get_start_point();
    p2t::Point* get_end_point();
    int get_selected_algorithm();
    int get_selected_map();

};

