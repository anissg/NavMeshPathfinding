#include "MapBuilder.h"
#include <windowsx.h>

MapBuilder::MapBuilder() : elements(), obstacles(), start_point(), end_point(), 
mouse_state(0), selected_item(-1), visible(true), reset(false), interface_state(0)
{
    texture_obstacle1 = Sprite(0, 0, 140 * .7, 88 * .7);
    texture_obstacle2 = Sprite(0, 0, 70 * .7, 88 * .7);
    texture_obstacle3 = Sprite(0, 0, 64 * .7, 64 * .7);
    texture_obstacle4 = Sprite(0, 0, 32 * .7, 32 * .7);
    texture_start =     Sprite(0, 0, 15, 21);
    texture_end =       Sprite(0, 0, 9, 11);
    texture_ground =    Sprite(0, 0, 128 * 2, 128 * 2);
}

MapBuilder::~MapBuilder()
{
}

void MapBuilder::init_textures(LPDIRECT3DDEVICE9 dx_device)
{
    texture_obstacle1.load_texture(dx_device, L"./Resources/house1.png");
    texture_obstacle2.load_texture(dx_device, L"./Resources/house2.png");
    texture_obstacle3.load_texture(dx_device, L"./Resources/tree1.png");
    texture_obstacle4.load_texture(dx_device, L"./Resources/tree2.png");
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

void MapBuilder::WindowProcedure(UINT msg, WPARAM wParam, LPARAM lParam)
{
    int x = GET_X_LPARAM(lParam);
    int y = SCREEN_HEIGHT - GET_Y_LPARAM(lParam);

    switch (msg)
    {
    case WM_LBUTTONDOWN:
        if (selected_item != -1)
        {
            selected_item = -1;
            mouse_state = 0;
        }
        break;
    case WM_MOUSEMOVE:
        if (mouse_state == 1)
        {
            mouse_state = 2;
            ITEM it;
            it.p = Point2(x, y);
            obstacle ob;
            switch (selected_item)
            {
            case 0:
                it.type = 0;
                elements.push_back(it);
                ob.push_back(Point2(x, y));
                ob.push_back(Point2(x + (int)texture_obstacle1.width, y));
                ob.push_back(Point2(x + (int)texture_obstacle1.width, y + (int)texture_obstacle1.height));
                ob.push_back(Point2(x, y + (int)texture_obstacle1.height));
                break;
            case 1:
                it.type = 1;
                elements.push_back(it);
                ob.push_back(Point2(x, y));
                ob.push_back(Point2(x + (int)texture_obstacle2.width, y));
                ob.push_back(Point2(x + (int)texture_obstacle2.width, y + (int)texture_obstacle2.height));
                ob.push_back(Point2(x, y + (int)texture_obstacle2.height));
                break;
            case 2:
                it.type = 2;
                elements.push_back(it);
                ob.push_back(Point2(x, y));
                ob.push_back(Point2(x + (int)texture_obstacle3.width, y));
                ob.push_back(Point2(x + (int)texture_obstacle3.width, y + (int)texture_obstacle3.height));
                ob.push_back(Point2(x, y + (int)texture_obstacle3.height));
                break;
            case 3:
                it.type = 3;
                elements.push_back(it);
                ob.push_back(Point2(x, y));
                ob.push_back(Point2(x + (int)texture_obstacle4.width, y));
                ob.push_back(Point2(x + (int)texture_obstacle4.width, y + (int)texture_obstacle4.height));
                ob.push_back(Point2(x, y + (int)texture_obstacle4.height));
                break;
            case 4:
                texture_start.x = x;
                texture_start.y = y;
                break;
            case 5:
                texture_end.x = x;
                texture_end.y = y;
                break;
            }
            obstacles.push_back(ob);

        }
        else if (mouse_state == 2)
        {
            int iIntersectOtherObstacle = -1;
            int m = obstacles.size() - 1;
            obstacle ob = obstacles[m];

            int iXMax1 = 0;
            int iXMin1 = 100000;
            int iYMax1 = 0;
            int iYMin1 = 100000;
            int iWidth = 0;
            int iHeight = 0;
            for (int k = 0; k < ob.size(); k++)
            {
                if (ob[k].x > iXMax1) iXMax1 = ob[k].x;
                if (ob[k].x < iXMin1) iXMin1 = ob[k].x;
                if (ob[k].y > iYMax1) iYMax1 = ob[k].y;
                if (ob[k].y < iYMin1) iYMin1 = ob[k].y;
            }
            iWidth = iXMax1 - iXMin1;
            iHeight = iYMax1 - iYMin1;

            for (int j = 0; j < obstacles.size() - 1; j++)
            {
                int iXMax = 0;
                int iXMin = 100000;
                int iYMax = 0;
                int iYMin = 100000;
                for (int k = 0; k < obstacles[j].size(); k++)
                {
                    if (obstacles[j][k].x > iXMax) iXMax = obstacles[j][k].x;
                    if (obstacles[j][k].x < iXMin) iXMin = obstacles[j][k].x;
                    if (obstacles[j][k].y > iYMax) iYMax = obstacles[j][k].y;
                    if (obstacles[j][k].y < iYMin) iYMin = obstacles[j][k].y;
                }

                if (((iXMax1 >= iXMin && iXMax1 <= iXMax) || (iXMin1 >= iXMin && iXMin1 <= iXMax)) &&
                    ((iYMax1 >= iYMin && iYMax1 <= iYMax) || (iYMin1 >= iYMin && iYMin1 <= iYMax)))
                {
                    if ((iXMin1 >= iXMin && iXMin1 <= iXMax))
                    {
                        x = iXMax + 1;
                    }
                    else if ((iXMax1 >= iXMin && iXMax1 <= iXMax))
                    {
                        x = iXMin - iWidth - 1;
                    }
                    else if ((iYMin1 >= iYMin && iYMin1 <= iYMax))
                    {
                        y = iYMax + 1;
                    }
                    else if ((iYMax1 >= iYMin && iYMax1 <= iYMax))
                    {
                        y = iYMin - iHeight - 1;
                    }
                }
            }

            int n = ob.size();
            switch (selected_item)
            {
            case 0:
                elements[elements.size() - 1].p = Point2(x, y);
                obstacles[m][n - 4] = Point2(x, y);
                obstacles[m][n - 3] = Point2(x + (int)texture_obstacle1.width, y);
                obstacles[m][n - 2] = Point2(x + (int)texture_obstacle1.width, y + (int)texture_obstacle1.height);
                obstacles[m][n - 1] = Point2(x, y + (int)texture_obstacle1.height);
                break;
            case 1:
                elements[elements.size() - 1].p = Point2(x, y);
                obstacles[m][n - 4] = Point2(x, y);
                obstacles[m][n - 3] = Point2(x + (int)texture_obstacle2.width, y);
                obstacles[m][n - 2] = Point2(x + (int)texture_obstacle2.width, y + (int)texture_obstacle2.height);
                obstacles[m][n - 1] = Point2(x, y + (int)texture_obstacle2.height);
                break;
            case 2:
                elements[elements.size() - 1].p = Point2(x, y);
                obstacles[m][n - 4] = Point2(x, y);
                obstacles[m][n - 3] = Point2(x + (int)texture_obstacle3.width, y);
                obstacles[m][n - 2] = Point2(x + (int)texture_obstacle3.width, y + (int)texture_obstacle3.height);
                obstacles[m][n - 1] = Point2(x, y + (int)texture_obstacle3.height);
                break;
            case 3:
                elements[elements.size() - 1].p = Point2(x, y);
                obstacles[m][n - 4] = Point2(x, y);
                obstacles[m][n - 3] = Point2(x + (int)texture_obstacle4.width, y);
                obstacles[m][n - 2] = Point2(x + (int)texture_obstacle4.width, y + (int)texture_obstacle4.height);
                obstacles[m][n - 1] = Point2(x, y + (int)texture_obstacle4.height);
                break;
            case 4:
                texture_start.x = x;
                texture_start.y = y;
                break;
            case 5:
                texture_end.x = x;
                texture_end.y = y;
                break;
            }
        }
        break;
    }
}

void MapBuilder::MouseEventPutStartPoint(UINT msg, WPARAM wParam, LPARAM lParam)
{
}

void MapBuilder::MouseEventPutEndPoint(UINT msg, WPARAM wParam, LPARAM lParam)
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

void MapBuilder::set_selected_item(int item)
{
    selected_item = item;
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
