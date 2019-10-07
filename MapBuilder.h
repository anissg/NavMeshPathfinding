#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <vector>
#include "poly2tri/poly2tri.h"
#include "DxStructs.h"
#include "NavMesh.h"
#include "Sprite.h"

class MapBuilder
{
    Sprite		            texture_obstacle1;
    Sprite		            texture_obstacle2;
    Sprite		            texture_obstacle3;
    Sprite		            texture_obstacle4;
    Sprite		            texture_start;
    Sprite		            texture_end;
    Sprite		            texture_ground;

    std::vector<ITEM>		elements;
    std::vector<obstacle>	obstacles;
    p2t::Point*             start_point;
    p2t::Point*             end_point;

    int					    mouse_state;
    int					    selected_items;
    bool				    visible;
    bool				    reset;
    int					    interface_state;

public:
    MapBuilder();
    ~MapBuilder();
    
    void init_textures(LPDIRECT3DDEVICE9 dx_device);
    void draw(LPDIRECT3DDEVICE9 dx_device);

    void WindowProcedure(UINT uint_Message, WPARAM wParam, LPARAM lParam);
    void MouseEventNavMesh(UINT uint_Message, WPARAM wParam, LPARAM lParam);
    void MouseEventPutItems(UINT uint_Message, WPARAM wParam, LPARAM lParam);
    void MouseEventPutStartPoint(UINT uint_Message, WPARAM wParam, LPARAM lParam);
    void MouseEventPutEndPoint(UINT uint_Message, WPARAM wParam, LPARAM lParam);

    std::vector<obstacle> get_obstacles();
    void set_obstacles(std::vector<obstacle> obstacles);
    void set_obstacles_types(std::vector<ITEM> elements);
    void set_interface_state(int state);
    void set_reset(bool reset);
    bool get_reset();
    int get_state();
    p2t::Point* get_start_point();
    p2t::Point* get_end_point();
};

