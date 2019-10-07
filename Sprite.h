#pragma once

#include <d3dx9.h>
#include "Texture.h"

class Sprite
{
    bool has_texture;
    Texture texture;

public:
    float x;
    float y;
    float width;
    float height;
    Sprite();
    Sprite(float x, float y, float width, float height);
    Sprite(float x, float y, float width, float height, float texture_width, float texture_height);
    ~Sprite(void);
    
    bool is_point_inside(float x, float y);
    void load_texture(LPDIRECT3DDEVICE9 dx_device, LPCWSTR file);
    void draw(LPDIRECT3DDEVICE9 dx_device);
    void draw(LPDIRECT3DDEVICE9 dx_device, float px, float py);
    void draw(LPDIRECT3DDEVICE9 dx_device, float px, float py, int n, int m);
};

