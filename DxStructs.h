#pragma once

#include "Point2.h"
#include <d3dx9math.h>

class CUSTOMVERTEX
{
public:
    float x, y, z;
    DWORD color;
};

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

struct ITEM 
{ 
    Point2 p; 
    int type; 
};