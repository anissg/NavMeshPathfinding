#pragma once

#include <windows.h>
#include <d3dx9.h>
#include "DxStructs.h"

class Texture
{
public:
    float width;
    float height;
    CUSTOMVERTEX tex_coords[6];
    LPDIRECT3DTEXTURE9 texture;
    LPDIRECT3DVERTEXBUFFER9 vertex_buffer;

    Texture();
    Texture(float width, float height);
    ~Texture();
    void generate_texture_rect();
    void load_texture(LPDIRECT3DDEVICE9 p_dx_Device, LPCWSTR file);
    void draw(LPDIRECT3DDEVICE9 dx_device, float x, float y);
    void draw(LPDIRECT3DDEVICE9 dx_device, float x, float y, int i, int j);
};
