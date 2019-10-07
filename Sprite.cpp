#include "Sprite.h"

Sprite::Sprite() : texture(), has_texture(false), x(0), y(0), width(0), height(0)
{
}

Sprite::Sprite(float x, float y, float width, float height) : texture(width, height), has_texture(false), x(x), y(y), width(width), height(height)
{
}

Sprite::Sprite(float x, float y, float width, float height, float texture_width, float texture_height) : texture(texture_width, texture_height), 
has_texture(false), x(x), y(y), width(width), height(height)
{
}

Sprite::~Sprite()
{
}

bool Sprite::is_point_inside(float x, float y)
{
    if (x >= this->x && x <= this->x + width && y >= this->y && y <= this->y + height)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Sprite::load_texture(LPDIRECT3DDEVICE9 dx_device, LPCWSTR file)
{
    texture.load_texture(dx_device, file);
    has_texture = true;
}

void Sprite::draw(LPDIRECT3DDEVICE9 dx_device)
{
    if (has_texture)
        texture.draw(dx_device, x, y);
}

void Sprite::draw(LPDIRECT3DDEVICE9 dx_device, float px, float py)
{
    if (has_texture)
    {
        texture.draw(dx_device, px, py);
    }
}

void Sprite::draw(LPDIRECT3DDEVICE9 dx_device, float px, float py, int n, int m)
{
    if (has_texture)
        texture.draw(dx_device, px, py, n, m);
}
