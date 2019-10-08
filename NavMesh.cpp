#include "NavMesh.h"
#include "Utils.h"

NavMesh::NavMesh() : CTD(), obstacles(obstacles), points_list(), points_list_2(),
triangles_list(), triangles_list_2(), edges_list(), vertex_buffer(), vertex_buffer_triangles(),
square_vertices(), material(), color(), nb_triangles(0), nb_points(0)
{
}

NavMesh::NavMesh(std::vector<Point2> points_list) : CTD(), obstacles(obstacles), points_list(), points_list_2(),
triangles_list(), triangles_list_2(), edges_list(), vertex_buffer(), vertex_buffer_triangles(),
square_vertices(), material(), nb_triangles(0)
{
    nb_points = 0;

    if (points_list.size() >= 3)
    {
        //look if the last edges insertions are replied.
        for (int i = 0; i < points_list.size(); i++)
        {
            p2t::Point* p = new p2t::Point(points_list[i].x, points_list[i].y);
            if (i == points_list.size() - 1)
            {
                points_list_2.push_back(p);
                for (int j = 0; j < points_list_2.size() - 1; j++)
                {
                    if (points_list[i].x == (int)points_list_2[j]->x && points_list[i].y == (int)points_list_2[j]->y)
                    {
                        points_list_2.pop_back();
                        break;
                    }
                }
            }
            else
            {
                points_list_2.push_back(p);
            }
        }
        CTD = new p2t::CDT(points_list_2);
    }
    else
    {
        for (int i = 0; i < points_list.size(); i++)
        {
            p2t::Point* p = new p2t::Point(points_list[i].x, points_list[i].y);
            points_list_2.push_back(p);
        }
    }
    
    color = 0xff000000;

    square_vertices[0].x = 0.0f;
    square_vertices[0].y = 5.0f;
    square_vertices[0].z = 0.0f;
    square_vertices[0].color = color;

    square_vertices[1].x = 5.0f;
    square_vertices[1].y = 0.0f;
    square_vertices[1].z = 0.0f;
    square_vertices[1].color = color;

    square_vertices[2].x = 0.0f;
    square_vertices[2].y = 0.0f;
    square_vertices[2].z = 0.0f;
    square_vertices[2].color = color;

    square_vertices[3].x = 5.0f;
    square_vertices[3].y = 5.0f;
    square_vertices[3].z = 0.0f;
    square_vertices[3].color = color;

    square_vertices[4].x = 5.0f;
    square_vertices[4].y = 0.0f;
    square_vertices[4].z = 0.0f;
    square_vertices[4].color = color;

    square_vertices[5].x = 0.0f;
    square_vertices[5].y = 5.0f;
    square_vertices[5].z = 0.0f;
    square_vertices[5].color = color;
}

NavMesh::~NavMesh()
{
}

void NavMesh::set_points(std::vector<Point2> obstacles)
{
    for (int i = 0; i < points_list.size(); i++)
    {
        p2t::Point* p = new p2t::Point(points_list[i].x, points_list[i].y);
        points_list_2.push_back(p);
    }
    CTD = new p2t::CDT(points_list_2);
}

void NavMesh::add_obstacle(std::vector<obstacle> obs)
{
    obstacles = obs;
    if (obstacles.size() > 0)
    {
        obstacle o;
        for (int i = 0; i < obstacles.size(); i++)
        {
            obstacle ob = obstacles[i];

            std::vector<p2t::Point*> obstacle_points;
            bool same_point = false;
            for (int i = 0; i < points_list_2.size(); i++)
            {
                for (int j = 0; j < ob.size(); j++)
                {
                    if (ob[j].x == (int)points_list_2[i]->x && ob[j].y == (int)points_list_2[i]->y)
                    {
                        same_point = true;
                        break;
                    }
                }
                if (same_point) break;
            }
            if (!same_point)
            {
                bool all_inside = true;
                for (int j = 0; j < ob.size(); j++)
                {

                    int iX = is_inside_polygon(ob[j]);

                    if (iX == 0)
                    {
                        all_inside = false;
                        break;
                    }
                }

                if (all_inside)
                {
                    for (int j = 0; j < ob.size(); j++)
                    {
                        p2t::Point* p = new p2t::Point(ob[j].x, ob[j].y);
                        obstacle_points.push_back(p);
                    }
                    CTD->AddHole(obstacle_points);
                }
            }
        }
    }
}

void NavMesh::update(LPDIRECT3DDEVICE9 dx_device)
{
    VOID* vertices = nullptr;
    dx_device->CreateVertexBuffer(
        6 * sizeof(CUSTOMVERTEX),
        0,
        D3DFVF_XYZ | D3DFVF_NORMAL,
        D3DPOOL_DEFAULT,
        &vertex_buffer,
        NULL);
    vertex_buffer->Lock(0, 6 * sizeof(CUSTOMVERTEX), (void**)& vertices, 0);
    memcpy(vertices, square_vertices, 6 * sizeof(CUSTOMVERTEX));
    vertex_buffer->Unlock();
    
    if (points_list_2.size() >= 3)
    {
        CTD->Triangulate();

        triangles_list_2.clear();
        triangles_list_2 = CTD->GetTriangles();

        nb_triangles = triangles_list_2.size();
        nb_points = triangles_list_2.size() * 3;

        D3DXVECTOR3* point = new D3DXVECTOR3[nb_points];

        int k = 0;
        for (int i = 0; i < nb_triangles; i++)
        {
            p2t::Triangle* t = triangles_list_2[i];
            point[k] = D3DXVECTOR3(t->GetPoint(0)->x, t->GetPoint(0)->y, 0);
            k++;
            point[k] = D3DXVECTOR3(t->GetPoint(1)->x, t->GetPoint(1)->y, 0);
            k++;
            point[k] = D3DXVECTOR3(t->GetPoint(2)->x, t->GetPoint(2)->y, 0);
            k++;
        }

        if (nb_points > 0)
        {
            dx_device->CreateVertexBuffer(
                nb_points * sizeof(CUSTOMVERTEX),
                0,
                D3DFVF_XYZ | D3DFVF_NORMAL,
                D3DPOOL_DEFAULT,
                &vertex_buffer_triangles,
                NULL);

            CUSTOMVERTEX* sommets = new CUSTOMVERTEX[nb_points];
            vertex_buffer_triangles->Lock(0, 0, (void**)& sommets, 0);
            for (int i = 0; i < nb_points; i++)
            {
                sommets[i].x = point[i].x;
                sommets[i].y = point[i].y;
                sommets[i].z = point[i].z;
                sommets[i].color = color;
            }
            vertex_buffer_triangles->Unlock();
        }
    }
}

void NavMesh::draw(LPDIRECT3DDEVICE9 dx_device)
{
    D3DXMATRIX position;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 0.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 0.0f;
    material.Ambient.a = 1.0f;

    if (nb_points >= 3)
    {
        dx_device->SetMaterial(&material);
        dx_device->SetStreamSource(0, vertex_buffer_triangles, 0, sizeof(CUSTOMVERTEX));
        dx_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
        dx_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        dx_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, nb_triangles);
    }

    for (int i = 0; i < triangles_list_2.size(); i++)
    {
        p2t::Point center = get_middle_point(triangles_list_2[i]);
        D3DXMatrixTranslation(&position, center.x - SCREEN_WIDTH / 2 - 5, center.y - SCREEN_HEIGHT / 2 - 5, 0.0f);
        dx_device->SetTransform(D3DTS_WORLD, &position);

        dx_device->SetStreamSource(0, vertex_buffer, 0, sizeof(CUSTOMVERTEX));
        dx_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
        dx_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        dx_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

        D3DXMatrixTranslation(&position, -1280.0f / 2, -768.0f / 2, 0.0f);
        dx_device->SetTransform(D3DTS_WORLD, &position);
    }
    for (int i = 0; i < points_list_2.size(); i++)
    {
        D3DXMatrixTranslation(&position, points_list_2[i]->x - SCREEN_WIDTH / 2 - 5, points_list_2[i]->y - SCREEN_HEIGHT / 2 - 5, 0.0f);
        dx_device->SetTransform(D3DTS_WORLD, &position);

        dx_device->SetStreamSource(0, vertex_buffer, 0, sizeof(CUSTOMVERTEX));
        dx_device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
        dx_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        dx_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

        D3DXMatrixTranslation(&position, -SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
        dx_device->SetTransform(D3DTS_WORLD, &position);
    }
}

int NavMesh::is_inside_polygon(Point2 center)
{
    std::vector<p2t::Point*> p = points_list_2;
    int i1, i2, n, N, S, S1, S2, S3;
    int flag = 0;
    N = p.size();
    for (n = 0; n < N; n++)
    {
        flag = 0;
        i1 = (n < N - 1) ? (n + 1) : 0;
        while (flag == 0)
        {
            i2 = i1 + 1;
            if (i2 >= N) i2 = 0;
            if (i2 == (n < N - 1 ? n + 1 : 0)) break;

            S = abs(p[i1]->x * (p[i2]->y - p[n]->y) + p[i2]->x * (p[n]->y - p[i1]->y) + p[n]->x * (p[i1]->y - p[i2]->y));
            S1 = abs(p[i1]->x * (p[i2]->y - center.y) + p[i2]->x * (center.y - p[i1]->y) + center.x * (p[i1]->y - p[i2]->y));
            S2 = abs(p[n]->x * (p[i2]->y - center.y) + p[i2]->x * (center.y - p[n]->y) + center.x * (p[n]->y - p[i2]->y));
            S3 = abs(p[i1]->x * (p[n]->y - center.y) + p[n]->x * (center.y - p[i1]->y) + center.x * (p[i1]->y - p[n]->y));
            if (S == S1 + S2 + S3)
            {
                flag = 1;
                break;
            }
            i1 = i1 + 1;
            if (i1 >= N) i1 = 0;
        }
        if (flag == 0)
            break;
    }
    return flag;
}

std::vector<p2t::Triangle*> NavMesh::get_mesh()
{
    return triangles_list_2;
}
