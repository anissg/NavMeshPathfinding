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

#define MAX_LOADSTRING 100
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 768

int game_state = 0;
int app_running = 1;

HINSTANCE hInst;

HWND Window(LPCTSTR title, int x_pos, int y_pos, int width, int height);
LPDIRECT3DDEVICE9 InitializeDevice(HWND hwnd);
void SetUpCamera(LPDIRECT3DDEVICE9 p_dx_Device);
void DrawScene(LPDIRECT3DDEVICE9 dx_device);
void Update(LPDIRECT3DDEVICE9 dx_device);

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    MSG msg_Message;
    HWND hwnd = Window(L"NavMeshPathfinding", 0, 0, SCREEN_WIDTH + 7, SCREEN_HEIGHT + 29); // the window offsets
    
    LPDIRECT3DDEVICE9 dx_device = InitializeDevice(hwnd);
    SetUpCamera(dx_device);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;

    ImGui::StyleColorsDark();
    
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(dx_device);

    while (app_running)
    {
        if (PeekMessage(&msg_Message, hwnd, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg_Message);
        }

        Update(dx_device);
        DrawScene(dx_device);
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
    /*switch (game_state)
    {
        case 1:
        {
            _floor->OurWindowProcedure(uint_Message, parameter1, parameter2);
            _selectItem->OurWindowProcedure(uint_Message, parameter1, parameter2);
            break;
        }
        case 2:
        {
            _selectItem->OurWindowProcedure(uint_Message, parameter1, parameter2);
        }
        case 3:
        {
            _selectItem->OurWindowProcedure(uint_Message, parameter1, parameter2);
        }
    }*/
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

LPDIRECT3DDEVICE9 InitializeDevice(HWND hwnd)
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

void SetUpCamera(LPDIRECT3DDEVICE9 p_dx_Device)
{
    D3DXMATRIX ortho2d;
    D3DXMATRIX id;

    D3DXMatrixOrthoLH(&ortho2d, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
    D3DXMatrixIdentity(&id);

    p_dx_Device->SetTransform(D3DTS_PROJECTION, &ortho2d);
    p_dx_Device->SetTransform(D3DTS_WORLD, &id);
    p_dx_Device->SetTransform(D3DTS_VIEW, &id);

    D3DXMATRIX pos;
    D3DXMatrixTranslation(&pos, -SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, 0.0f);
    p_dx_Device->SetTransform(D3DTS_WORLD, &pos);
}

void DrawScene(LPDIRECT3DDEVICE9 dx_device)
{
    dx_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    dx_device->BeginScene();

    switch (game_state)
    {
    case 0:
        break;
    case 1:
        //_selectItem->Draw(p_dx_Device);
        //_navMesh->Draw(p_dx_Device);
        break;
    case 2:
        //_selectItem->Draw(p_dx_Device);
        //_navMesh->Draw(p_dx_Device);
        //_pSearchAlgorithm->Draw(p_dx_Device);
        break;
    case 3:
        //_selectItem->Draw(p_dx_Device);
        //_navMesh->Draw(p_dx_Device);

        //if (_selectItem->GetState() >= 4)
        //{
        //    _pSearchAlgorithm->Draw(p_dx_Device);
        //}
        break;
    }

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    dx_device->EndScene();
    dx_device->Present(NULL, NULL, NULL, NULL);
}

void Update(LPDIRECT3DDEVICE9 dx_device)
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    if (ImGui::Button("Button"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    switch (game_state)
    {
    case 0://init floor
        //_floor = new CComposeFloor;
        //_navMesh = new CNavmesh(_floor->getListPoints());
        //_navMesh->Update(p_dx_Device);
        //_selectItem = new CSelectItem();
        //_selectItem->InitTexture(p_dx_Device);
        game_state = 1;
        break;
    case 1:
        //iSelectedMap = _selectItem->GetSelectedMap();
        //if (iSelectedMap >= 0)
        //{
        //    _floor->SetDefaultMap(iSelectedMap);
        //    _selectItem->SetObstacles(_floor->GetObstacles(iSelectedMap));
        //    _selectItem->SetObstaclesTypes(_floor->GetObstaclesTypes(iSelectedMap));
        //    _selectItem->SetStateInterfae(1);
        //    iStateGame = 2;
        //}
        //else
        //{
        //    if (_floor->complete())
        //    {
        //        _selectItem->SetStateInterfae(1);
        //        iStateGame = 2;
        //    }
            /*_navMesh = new CNavmesh(_floor->getListPoints());
            _navMesh->Update(p_dx_Device);
            switch (_selectItem->GetSelectedAlgorithm())
            {
            case 0:
                _pSearchAlgorithm = new CSearchBreadFirst();
                break;
            case 1:
                _pSearchAlgorithm = new CSearchDeepFirst();
                break;
            case 2:
                _pSearchAlgorithm = new CSearchBidirectional();
                break;
            case 3:
                _pSearchAlgorithm = new CSearchAStar();
                break;
            case 4:
                _pSearchAlgorithm = new CSearchGreedy();
                break;
            }
            _pSearchAlgorithm->SetMesh(_navMesh->GetMesh());
            _pSearchAlgorithm->SetPosInit(_selectItem->GetStartPoint());
            _pSearchAlgorithm->SetEndPoint(_selectItem->GetEndPoint());
            _pSearchAlgorithm->GetPath();
            _pSearchAlgorithm->Update(p_dx_Device);
            if (_selectItem->GetReset())
            {
                iStateGame = 0;
                _selectItem->SetReset(false);
            }
        }*/
        break;
    case 2:
        /*iSelectedMap = _selectItem->GetSelectedMap();
        if (iSelectedMap >= 0)
        {
            _floor->SetDefaultMap(iSelectedMap);
            _selectItem->SetObstacles(_floor->GetObstacles(iSelectedMap));
            _selectItem->SetObstaclesTypes(_floor->GetObstaclesTypes(iSelectedMap));
            _selectItem->SetStateInterfae(1);
        }
        else
        {
            _navMesh = new CNavmesh(_floor->getListPoints());
            _navMesh->addObstacle(_selectItem->GetObstacles());
            _navMesh->Update(p_dx_Device);
            if (_selectItem->GetReset())
            {
                iStateGame = 0;
                _selectItem->SetReset(false);
            }
            switch (_selectItem->GetSelectedAlgorithm())
            {
            case 0:
                _pSearchAlgorithm = new CSearchBreadFirst();
                break;
            case 1:
                _pSearchAlgorithm = new CSearchDeepFirst();
                break;
            case 2:
                _pSearchAlgorithm = new CSearchBidirectional();
                break;
            case 3:
                _pSearchAlgorithm = new CSearchAStar();
                break;
            case 4:
                _pSearchAlgorithm = new CSearchGreedy();
                break;
            }
            _pSearchAlgorithm->SetMesh(_navMesh->GetMesh());
            _pSearchAlgorithm->SetPosInit(_selectItem->GetStartPoint());
            _pSearchAlgorithm->SetEndPoint(_selectItem->GetEndPoint());
            _pSearchAlgorithm->GetPath();
            _pSearchAlgorithm->Update(p_dx_Device);
        }*/
        break;
    }
}