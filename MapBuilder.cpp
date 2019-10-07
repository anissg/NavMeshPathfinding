#include "MapBuilder.h"

MapBuilder::MapBuilder() : elements(), obstacles(), start_point(), end_point(), 
mouse_state(0), selected_items(0), visible(true), reset(false), interface_state(0)
{
    texture_obstacle1 = Sprite(0, 0, 64, 64);
    texture_obstacle2 = Sprite(0, 0, 32, 32);
    texture_obstacle3 = Sprite(0, 0, 140, 88);
    texture_obstacle4 = Sprite(0, 0, 70, 88);
    texture_start =     Sprite(0, 0, 15, 21);
    texture_end =       Sprite(0, 0, 9, 11);
    texture_ground =    Sprite(0, 0, 128 * 2, 128 * 2);
}

MapBuilder::~MapBuilder()
{
}

void MapBuilder::init_textures(LPDIRECT3DDEVICE9 dx_device)
{
    texture_obstacle1.load_texture(dx_device, L"./Resources/tree1.png");
    texture_obstacle2.load_texture(dx_device, L"./Resources/tree2.png");
    texture_obstacle3.load_texture(dx_device, L"./Resources/house1.png");
    texture_obstacle4.load_texture(dx_device, L"./Resources/house2.png");
    texture_start.load_texture(dx_device, L"./Resources/player.png");
    texture_end.load_texture(dx_device, L"./Resources/potion.png");
    texture_ground.load_texture(dx_device, L"./Resources/background.png");
}

void MapBuilder::draw(LPDIRECT3DDEVICE9 dx_device)
{
    texture_ground.draw(dx_device, texture_ground.x, texture_ground.y, 5, 3);
    
    switch (interface_state)
    {
    case 1:
        for (int i = 0; i < elements.size(); i++)
        {
            switch (elements[i].type)
            {
            case 0:
                texture_obstacle1.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            case 1:
                texture_obstacle2.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            case 2:
                texture_obstacle3.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            case 3:
                texture_obstacle4.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            }
        }
        break;
    case 2:
        texture_start.draw(dx_device, start_point->x, start_point->y);
    case 3:
        for (int i = 0; i < elements.size(); i++)
        {
            switch (elements[i].type)
            {
            case 0:
                texture_obstacle1.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            case 1:
                texture_obstacle2.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            case 2:
                texture_obstacle3.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            case 3:
                texture_obstacle4.draw(dx_device, elements[i].p.x, elements[i].p.y);
                break;
            }
        }
        break;
    }
}

void MapBuilder::WindowProcedure(UINT uint_Message, WPARAM wParam, LPARAM lParam)
{
}

void MapBuilder::MouseEventNavMesh(UINT uint_Message, WPARAM wParam, LPARAM lParam)
{
}

void MapBuilder::MouseEventPutItems(UINT uint_Message, WPARAM wParam, LPARAM lParam)
{
}

void MapBuilder::MouseEventPutStartPoint(UINT uint_Message, WPARAM wParam, LPARAM lParam)
{
}

void MapBuilder::MouseEventPutEndPoint(UINT uint_Message, WPARAM wParam, LPARAM lParam)
{
}

std::vector<obstacle> MapBuilder::get_obstacles()
{
    return obstacles;
}

void MapBuilder::set_obstacles(std::vector<obstacle> obstacles)
{
    this->obstacles = obstacles;
}

void MapBuilder::set_obstacles_types(std::vector<ITEM> elements)
{
    this->elements = elements;
}

void MapBuilder::set_interface_state(int state)
{
    interface_state = state;
}

void MapBuilder::set_reset(bool reset)
{
    this->reset = reset;
}

bool MapBuilder::get_reset()
{
    return reset;
}

int MapBuilder::get_state()
{
    return interface_state;
}

p2t::Point* MapBuilder::get_start_point()
{
    return start_point;
}

p2t::Point* MapBuilder::get_end_point()
{
    return end_point;
}
