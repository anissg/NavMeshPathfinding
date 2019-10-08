#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <windows.h> 
#include <windowsx.h>
#include <d3d9.h> 
#include <d3dx9.h> 

#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"

#include "NavMesh.h"
#include "MapBuilder.h"
#include "Map.h"
#include "DxStructs.h"
#include "SearchAlgo.h"
#include "CSearchDjikstra.h"
#include "CSearchAStar.h"

int game_state = 0;
int app_running = 1;
int selected_map = -1;
int selected_item = -1;
int selected_algo = 0;
bool draw_navmesh = true;
bool draw_visited_nodes = false;
bool edit_mode = true;
NavMesh* nav_mesh;
MapBuilder* map_builder;
Map* map;
SearchAlgo* search_algo;

HINSTANCE hInst;
LPDIRECT3DDEVICE9 dx_device;
ImGuiWindowFlags window_flags;

HWND Window(LPCTSTR title, int x_pos, int y_pos, int width, int height);
LPDIRECT3DDEVICE9 initialize_device(HWND hwnd);
void setup_camera(LPDIRECT3DDEVICE9 p_dx_Device);
void initialize();
void update(LPDIRECT3DDEVICE9 dx_device);
void draw_scene(LPDIRECT3DDEVICE9 dx_device);

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    MSG msg_Message;
    HWND hwnd = Window(L"NavMeshPathfinding", 0, 0, SCREEN_WIDTH + 7, SCREEN_HEIGHT + 29); // the window offsets
    
    dx_device = initialize_device(hwnd);
    setup_camera(dx_device);
    initialize();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io;

    ImGui::StyleColorsDark();
    
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(dx_device);

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 728), ImGuiCond_FirstUseEver);

    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding.x = 8.0f;
    style.FramePadding.y = 5.0f;
    style.ItemSpacing.y = 10.0f;

    window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    while (app_running)
    {
        if (PeekMessage(&msg_Message, hwnd, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg_Message);
        }

        update(dx_device);
        draw_scene(dx_device);
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    dx_device->Release();

    DestroyWindow(hwnd);

    return 0;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
        case WM_KEYDOWN:
        {
            app_running = 0;
            break;
        }
        case WM_RBUTTONUP:
        {
        }
        case WM_CLOSE:
        {
            app_running = 0;
            break;
        }
    }

    switch (game_state)
    {
        case 1:
        {
            map->WindowProcedure(msg, wParam, lParam);
            map_builder->WindowProcedure(msg, wParam, lParam);
            break;
        }
        case 2:
        case 3:
        {
            map_builder->WindowProcedure(msg, wParam, lParam);
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND Window(LPCTSTR title, int x_pos, int y_pos, int width, int height)
{
    WNDCLASSEX wnd_Structure;

    wnd_Structure.cbSize = sizeof(WNDCLASSEX);
    wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
    wnd_Structure.lpfnWndProc = MainWindowProc;
    wnd_Structure.cbClsExtra = 0;
    wnd_Structure.cbWndExtra = 0;
    wnd_Structure.hInstance = GetModuleHandle(NULL);
    wnd_Structure.hIcon = NULL;
    wnd_Structure.hCursor = NULL;
    wnd_Structure.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wnd_Structure.lpszMenuName = NULL;
    wnd_Structure.lpszClassName = L"WindowClassName";
    wnd_Structure.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wnd_Structure);

    return CreateWindowEx(
        WS_EX_CONTROLPARENT, 
        L"WindowClassName", 
        title,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 
        x_pos,
        y_pos,
        width,
        height,
        NULL, 
        NULL, 
        GetModuleHandle(NULL), 
        NULL);
}

LPDIRECT3DDEVICE9 initialize_device(HWND hwnd)
{
    LPDIRECT3D9 dx_object;
    LPDIRECT3DDEVICE9 dx_device;

    dx_object = Direct3DCreate9(D3D_SDK_VERSION);
    if (dx_object == NULL)
    {
        MessageBox(hwnd, L"DirectX Runtime library not installed!", L"InitializeDevice()", MB_OK);
        exit(-1);
    }

    D3DPRESENT_PARAMETERS dx_pres_params;
    ZeroMemory(&dx_pres_params, sizeof(dx_pres_params));
    dx_pres_params.Windowed = TRUE;
    dx_pres_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    dx_pres_params.BackBufferFormat = D3DFMT_UNKNOWN;

    if (FAILED(dx_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_pres_params, &dx_device)))
    {
        if (FAILED(dx_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dx_pres_params, &dx_device)))
        {
            MessageBox(hwnd, L"Failed to create even the reference device!", L"InitializeDevice()", MB_OK);
            exit(-1);
        }
    }

    dx_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    dx_device->SetRenderState(D3DRS_LIGHTING, false);

    return dx_device;
}

void setup_camera(LPDIRECT3DDEVICE9 dx_device)
{
    D3DXMATRIX ortho2d;
    D3DXMATRIX id;

    D3DXMatrixOrthoLH(&ortho2d, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
    D3DXMatrixIdentity(&id);

    dx_device->SetTransform(D3DTS_PROJECTION, &ortho2d);
    dx_device->SetTransform(D3DTS_WORLD, &id);
    dx_device->SetTransform(D3DTS_VIEW, &id);

    D3DXMATRIX pos;
    D3DXMatrixTranslation(&pos, -SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
    dx_device->SetTransform(D3DTS_WORLD, &pos);
}

void initialize() {
    map = new Map();
    nav_mesh = new NavMesh(/*floor->getListPoints()*/);
    nav_mesh->update(dx_device);
    map_builder = new MapBuilder();
    map_builder->init_textures(dx_device);
    game_state = 1;
}

void update(LPDIRECT3DDEVICE9 dx_device)
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Settings", nullptr, window_flags);
    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::CollapsingHeader("Maps"))
    {
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.5f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.5f, 0.8f, 0.8f));
        if (ImGui::Button("Demo #1"))
            selected_map = 0;
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine(); 
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.8f, 0.8f));
        if (ImGui::Button("Demo #2"))
            selected_map = 1;
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.9f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.9f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.9f, 0.8f, 0.8f));
        if (ImGui::Button("Demo #3"))
            selected_map = 2;
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        if (ImGui::Button("New Map"))
        {
            selected_map = -1;
            game_state = 1;
            map_builder->set_reset(false);
        }
    }
    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::CollapsingHeader("NavMesh"))
    {
        if (edit_mode)
        {
            ImGui::PushID(1);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.44f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.44f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.44f, 0.8f, 0.8f));
            if (ImGui::Button("End Mesh Edit"))
                edit_mode = false;
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        else
        {
            ImGui::PushID(1);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.3f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.3f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.3f, 0.8f, 0.8f));
            if (ImGui::Button("Start Mesh Edit"))
                edit_mode = true;
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
    }
    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::CollapsingHeader("Obstacles"))
    {
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.65f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.65f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.65f, 0.8f, 0.8f));
        if (ImGui::Button("Tree1"))
        {
            selected_item = 0;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine(); 
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 0.8f, 0.8f));
        if (ImGui::Button("Tree2"))
        {
            selected_item = 1;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.75f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.75f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.75f, 0.8f, 0.8f));
        if (ImGui::Button("House1"))
        {
            selected_item = 2;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.8f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.8f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.8f, 0.8f, 0.8f));
        if (ImGui::Button("House2"))
        {
            selected_item = 3;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();

    }
    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::CollapsingHeader("Start / End positions"))
    {
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.3f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.3f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.3f, 0.8f, 0.8f));
        if (ImGui::Button("Start"))
        {

        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID(1);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
        if (ImGui::Button("End"))
        {

        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }
    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::CollapsingHeader("Algorithms"))
    {
        ImGui::RadioButton("Djikstra", &selected_algo, 0);
        ImGui::RadioButton("A Star", &selected_algo, 1);
    }
    ImGui::SetNextTreeNodeOpen(true);
    if (ImGui::CollapsingHeader("Params"))
    {
        ImGui::Checkbox("Draw NavMesh", &draw_navmesh);
        //ImGui::Checkbox("Draw Visited Nodes", &draw_visited_nodes);
    }
    ImGui::End();



    switch (game_state)
    {
    case 1:
        if (selected_map >= 0)
        {
            map->set_demo_map(selected_map);
            map_builder->set_obstacles(map->get_obstacles(selected_map));
            map_builder->set_obstacles_types(map->get_obstacles_types(selected_map));
            map_builder->set_interface_state(1);
            game_state = 2;
            selected_map = -1;
        }
        else
        {
            map->set_complete(!edit_mode);
            if (!edit_mode)
            {
                map_builder->set_interface_state(1);
                game_state = 2;
            }
            nav_mesh = new NavMesh(map->get_points_list());
            nav_mesh->update(dx_device);
            switch (selected_algo)
            {
            case 0:
                search_algo = new CSearchDjikstra();
                break;
            case 1:
                search_algo = new CSearchAStar();
                break;
            }
            search_algo->SetMesh(nav_mesh->get_mesh());
            search_algo->SetPosInit(map_builder->get_start_point());
            search_algo->SetEndPoint(map_builder->get_end_point());
            search_algo->GetPath();
            search_algo->Update(dx_device);
        }
        break;
    case 2:
        if (selected_map >= 0)
        {
            map->set_demo_map(selected_map);
            map_builder->set_obstacles(map->get_obstacles(selected_map));
            map_builder->set_obstacles_types(map->get_obstacles_types(selected_map));
            map_builder->set_interface_state(1);
            selected_map = -1;
        }
        else
        {
            if (edit_mode)
            {
                game_state = 1;
            }
            map_builder->set_selected_item(selected_item);
            nav_mesh = new NavMesh(map->get_points_list());
            nav_mesh->add_obstacle(map_builder->get_obstacles());
            nav_mesh->update(dx_device);
            switch (selected_algo)
            {
            case 0:
                search_algo = new CSearchDjikstra();
                break;
            case 1:
                search_algo = new CSearchAStar();
                break;
            }
            search_algo->SetMesh(nav_mesh->get_mesh());
            search_algo->SetPosInit(map_builder->get_start_point());
            search_algo->SetEndPoint(map_builder->get_end_point());
            search_algo->GetPath();
            search_algo->Update(dx_device);
        }
        break;
    }
}

void draw_scene(LPDIRECT3DDEVICE9 dx_device)
{
    dx_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    dx_device->BeginScene();

    switch (game_state)
    {
    case 1:
        map_builder->draw(dx_device);
        nav_mesh->draw(dx_device);
        break;
    case 2:
        map_builder->draw(dx_device);
        nav_mesh->draw(dx_device);
        search_algo->Draw(dx_device);
        break;
    case 3:
        map_builder->draw(dx_device);
        nav_mesh->draw(dx_device);

        if (map_builder->get_state() >= 4)
        {
            search_algo->Draw(dx_device);
        }
        break;
    }

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    dx_device->EndScene();
    dx_device->Present(NULL, NULL, NULL, NULL);
}