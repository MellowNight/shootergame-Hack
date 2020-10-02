#define IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
#include "Menu.h"

/*  global class for controlling the menu   */
MenuControl* menuControl = new MenuControl;



static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND hwnd;

MARGINS gMargin = { 0, 0, 600, 600 };
MARGINS zero = { -1, -1, -1, -1 };


typedef struct _ScreenInfo {
    int width;
    int height;
    int realWidth;
    int realHeight;
} ScreenInfo;

ScreenInfo GetScreenInfo()
{
    ScreenInfo sci = { 0 };
    sci.width = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXSCREEN) * 0.20;
    sci.height = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYSCREEN) * 0.20;
    sci.realWidth = GetSystemMetrics(SM_CXSCREEN);
    sci.realHeight = GetSystemMetrics(SM_CYSCREEN);
    return sci;
}




void    displayInfo()
{
    int         actorCount;
    float		health;
    float       runningSpeed;

    actorCount = Commands::read<int>(Globals->processHandle, (localGameInfo->persistentLevel + Offsets::actorCount));

    health = Commands::read<float>(Globals->processHandle, (localGameInfo->localPawn + Offsets::pawnHealth));

    runningSpeed = Commands::read<float>(Globals->processHandle, (localGameInfo->localPawn + Offsets::RunningSpeedModifier));


    DWORD64 aCameraManager = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->playerController + Offsets::CameraManager));
    vector3D	angle = Commands::read<vector3D>(Globals->processHandle,
        (aCameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraRotation), &angle);

    ImGui::Text("actor count %i ", actorCount);
    ImGui::Text("current health %f", health);
    ImGui::Text("current running speed modifier %f ", runningSpeed);
    ImGui::Text("X location %f, Y location %f, Z location %f", localGameInfo->position.x, 
        localGameInfo->position.y, localGameInfo->position.z);
    ImGui::Text("yaw %f, pitch %f, roll %f",  angle.x,
       angle.y, angle.z);
    ImGui::Text("current actor ID %i", localGameInfo->localactorId);
}

void SetStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(50, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(14, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 10.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 2.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.91f, 0.32f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.00f, 0.10f, 1.00f);
    style->Alpha = 1.0f;
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.34f, 0.15f, 0.30f, 1.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.17f, 0.00f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.34f, 0.15f, 0.30f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.34f, 0.15f, 0.30f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.34f, 0.15f, 0.30f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.34f, 0.15f, 0.30f, 1.00f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.34f, 0.15f, 0.30f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    style->Colors[ImGuiCol_Tab] = ImVec4(0.17f, 0.00f, 0.11f, 1.00f);
    style->Colors[ImGuiCol_TabActive] = ImVec4(0.36f, 0.15f, 0.31f, 1.00f);
    style->Colors[ImGuiCol_TabHovered] = ImVec4(0.36f, 0.15f, 0.31f, 1.00f);


    style->FrameRounding = 4.0f;
    style->WindowRounding = 5.0f;

}

void ClickThrough(bool click)
{
    if (click)
    {
        SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
    }
    else
    {
        SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_LAYERED);
    }
}


void MenuShow();

static bool ShowMenu = true;
static int LastTick = 0;
static int WorldTick = 0;
int    OverlayLoop()
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("b"), NULL };
    ::RegisterClassEx(&wc);
    hwnd = ::CreateWindow(wc.lpszClassName, _T("a"), WS_EX_TOPMOST | WS_POPUP, GetScreenInfo().realWidth * 0.05, 
        GetScreenInfo().realHeight * 0.05, GetScreenInfo().width, GetScreenInfo().height, NULL, NULL, wc.hInstance, NULL);

    //SetWindowLong(hwnd, GWL_EXSTYLE,(int)GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    ClickThrough(true);
    //SetLayeredWindowAttributes(hwnd, RGB(0,0,0), 0, ULW_COLORKEY);
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    gMargin = { 0, 0, GetScreenInfo().width, GetScreenInfo().height };

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 0;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.IniFilename = NULL;

    Globals->windowWidth = GetScreenInfo().width;
    Globals->windowHeight = GetScreenInfo().height;

    Globals->RealWindowWidth = GetScreenInfo().realWidth;
    Globals->RealWindowHeight = GetScreenInfo().realHeight;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    SetStyle();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);


    ImFont* font = io.Fonts->AddFontFromFileTTF(("c:\\Windows\\Fonts\\Arial.ttf"), 15.0f);
    //IM_ASSERT(font != NULL);


    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {

        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }
        ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        if (GetKeyState(VK_INSERT) & 0x8000)
        {
            if ((GetTickCount() - LastTick) > 200)
            {
                ShowMenu = !ShowMenu;
                LastTick = GetTickCount();
            }
        }


        if (ShowMenu)
        {
            // ImGui::ShowDemoWindow();
            SetStyle();
            MenuShow();

            ClickThrough(false);
        }
        else
        {
            ImVec4* colors = ImGui::GetStyle().Colors;
            colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            Render::Start();

            SetWindowPos(hwnd, HWND_TOPMOST, Globals->positionX, Globals->positionY,
                Globals->windowWidth, Globals->windowHeight, SWP_NOSIZE);

            ESP(menuControl->playerESP, menuControl->ItemESP, menuControl->ActorIdESP);
         

            Render::End();
            ClickThrough(true);

        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}


// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_PAINT:
        DwmExtendFrameIntoClientArea(hWnd, &zero);
        break;
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}



void MenuShow()
{
    ImGui::SetNextWindowSize(ImVec2(480, 520));

    {

        ImGui::Begin("OMANHOOK.cc shootergame UE4", 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);


        ImGui::BeginTabBar("Settings#left_tabs_bar");

        if (ImGui::BeginTabItem("Visuals")) {

            ImGui::Text("ESP settings");

            ImGui::Spacing();

            ImGui::Checkbox("player box ESP", &menuControl->playerESP);
            ImGui::Checkbox("Item ESP", &menuControl->ItemESP);
            ImGui::Checkbox("show actor IDs", &menuControl->ActorIdESP);
            ImGui::Checkbox("show ESP color options: ", &menuControl->showESPcolor);

            if (menuControl->showESPcolor)
            {
                ImGui::ColorPicker3("ESP color: ", menuControl->ESPcolor);
            }
            ImGui::EndTabItem();

        }

        if (ImGui::BeginTabItem("Aimbot")) {

            ImGui::Text("Aimbot");

            ImGui::Checkbox("Aimbot", &menuControl->aimbot);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Misc")) {

            ImGui::Text("Other features");
            ImGui::Spacing();



            ImGui::Checkbox("Godmode", &menuControl->godMode);
            ImGui::SameLine(); HelpMarker("lock health value");


            ImGui::Checkbox("infinite ammo", &menuControl->infinitAmmo);
            ImGui::SameLine(); HelpMarker("lock ammo value");


            ImGui::Checkbox("High jump", &menuControl->highJump);
            ImGui::Checkbox("Scope in", &menuControl->scopeIn);

            ImGui::Checkbox("high speed ", &menuControl->highSpeed);

            if (menuControl->highSpeed == true)
            {
                ImGui::SliderFloat("speed multiplier", &menuControl->runningSpeed, 0.0f, 20.0f);
            }

            ImGui::Spacing();

            displayInfo();


            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();


        ImGui::End();

    }
}