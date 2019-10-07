#include "Texture.h"

Texture::Texture() : width(0), height(0), tex_coords(), texture(), vertex_buffer()
{
}

Texture::Texture(float width, float height) : width(width), height(height)
{
    generate_texture_rect();
}

Texture::~Texture()
{
}

void Texture::generate_texture_rect()
{
    DWORD color = 0xffffffff;
    tex_coords[0].x = 0.0f;
    tex_coords[0].y = height;
    tex_coords[0].z = 0.0f;
    tex_coords[0].color = color;
    tex_coords[0].u = 0.0f;
    tex_coords[0].v = 0.0f;

    tex_coords[1].x = width;
    tex_coords[1].y = 0.0f;
    tex_coords[1].z = 0.0f;
    tex_coords[1].color = color;
    tex_coords[1].u = 1.0f;
    tex_coords[1].v = 1.0f;

    tex_coords[2].x = 0.0f;
    tex_coords[2].y = 0.0f;
    tex_coords[2].z = 0.0f;
    tex_coords[2].color = color;
    tex_coords[2].u = 0.0f;
    tex_coords[2].v = 1.0f;


    tex_coords[3].x = width;
    tex_coords[3].y = height;
    tex_coords[3].z = 0.0f;
    tex_coords[3].color = color;
    tex_coords[3].u = 1.0f;
    tex_coords[3].v = 0.0f;

    tex_coords[4].x = width;
    tex_coords[4].y = 0.0f;
    tex_coords[4].z = 0.0f;
    tex_coords[4].color = color;
    tex_coords[4].u = 1.0f;
    tex_coords[4].v = 1.0f;

    tex_coords[5].x = 0.0f;
    tex_coords[5].y = height;
    tex_coords[5].z = 0.0f;
    tex_coords[5].color = color;
    tex_coords[5].u = 0.0f;
    tex_coords[5].v = 0.0f;
}

void Texture::load_texture(LPDIRECT3DDEVICE9 dx_device, LPCWSTR file)
{
    VOID* vertices = nullptr;
    
    if (FAILED(D3DXCreateTextureFromFile(dx_device, file, &texture)))
    {
        MessageBox(nullptr, L"Cannot load the texture file!", L"load_texture()", MB_OK);
        exit(-1);
    }
        
    if (FAILED(dx_device->CreateVertexBuffer(
        6 * sizeof(CUSTOMVERTEX),
        0,
        CUSTOMFVF,
        D3DPOOL_DEFAULT,
        &vertex_buffer,
        NULL)))
    {
        MessageBox(nullptr, L"Cannot create vertex buffer!", L"load_texture()", MB_OK);
            exit(-1);
    }

    vertex_buffer->Lock(0, 6 * sizeof(CUSTOMVERTEX), (void**)& vertices, 0);
    memcpy(vertices, tex_coords, 6 * sizeof(CUSTOMVERTEX));
    vertex_buffer->Unlock();
}

void Texture::draw(LPDIRECT3DDEVICE9 dx_device, float x, float y)
{
    D3DXMATRIX position;
    dx_device->SetTexture(0, texture);
    D3DXMatrixTranslation(&position, x - SCREEN_WIDTH / 2, y - SCREEN_HEIGHT / 2, 0.0f);
    dx_device->SetTransform(D3DTS_WORLD, &position);
    dx_device->SetStreamSource(0, vertex_buffer, 0, sizeof(CUSTOMVERTEX));
    dx_device->SetFVF(CUSTOMFVF);
    dx_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    dx_device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000008f);
    dx_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    dx_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    dx_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    D3DXMatrixTranslation(&position, -SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
    dx_device->SetTransform(D3DTS_WORLD, &position);
    dx_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void Texture::draw(LPDIRECT3DDEVICE9 dx_device, float x, float y, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            D3DXMATRIX position;
            dx_device->SetTexture(0, texture);
            D3DXMatrixTranslation(&position, x + i * width - SCREEN_WIDTH / 2, y + j * height - SCREEN_HEIGHT / 2, 0.0f);
            dx_device->SetTransform(D3DTS_WORLD, &position);
            dx_device->SetStreamSource(0, vertex_buffer, 0, sizeof(CUSTOMVERTEX));
            dx_device->SetFVF(CUSTOMFVF);
            dx_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
            dx_device->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x0000008f);
            dx_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
            dx_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
            dx_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
            D3DXMatrixTranslation(&position, -1280.0f / 2, -768.0f / 2, 0.0f);
            dx_device->SetTransform(D3DTS_WORLD, &position);
            dx_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
        }
    }
}
