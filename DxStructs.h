#pragma once

#include "Point2.h"
#include <d3dx9math.h>
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 768

typedef std::vector<Point2>	obstacle;

struct SOMMETMESH
{
    D3DXVECTOR3 position;
    SOMMETMESH() {};
    inline SOMMETMESH(D3DXVECTOR3 pos)
    {
        position = pos;
    }
};

#define D3DFVF_SOMMETMESH (D3DFVF_XYZ|D3DFVF_NORMAL)

struct CUSTOMVERTEX
{
    FLOAT x, y, z;
    DWORD color;
    FLOAT u, v;
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct ITEM 
{ 
    Point2 p; 
    int type; 
};