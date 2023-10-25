/*
 * Skeleton IMGUI
 */

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

static ID3D11Device           *D3DDevice            = NULL;
static ID3D11DeviceContext    *D3DDeviceContext     = NULL;
static IDXGISwapChain         *SwapChain            = NULL;
static UINT                    ResizeWidth          = 0;
static UINT                    ResizeHeight         = 0;
static ID3D11RenderTargetView *MainRenderTargetView = NULL;

bool CreateDeviceD3D(HWND);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

// Main code
int main(int, char **)
{
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW WindowClass =
    {
      sizeof(WindowClass),
      CS_CLASSDC,
      WndProc,
      0L,
      0L,
      GetModuleHandle(NULL),
      NULL,
      NULL,
      NULL,
      NULL,
      L"Sockets Example",
      NULL
    };

    RegisterClassExW(&WindowClass);
    HWND WindowHandle = CreateWindowW(WindowClass.lpszClassName,
                                      L"Dear ImGui DirectX11 Example",
                                      WS_OVERLAPPEDWINDOW,
                                      100,
                                      100,
                                      1280,
                                      800,
                                      NULL,
                                      NULL,
                                      WindowClass.hInstance,
                                      NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(WindowHandle))
    {
        CleanupDeviceD3D();
        UnregisterClassW(WindowClass.lpszClassName, WindowClass.hInstance);
        return(1);
    }

    ShowWindow(WindowHandle, SW_SHOWDEFAULT);
    UpdateWindow(WindowHandle);

    // NOTE(ocornut): Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO *IO = &ImGui::GetIO();
    IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    IO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplWin32_Init(WindowHandle);
    ImGui_ImplDX11_Init(D3DDevice, D3DDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool ShowDemoWindow = true;
    bool ShowAnotherWindow = false;
    ImVec4 ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool Done = false;
    while (!Done)
    {
        MSG Message;
        while (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
            if (Message.message == WM_QUIT)
            {
                Done = true;
            }
        }

        if (Done)
        {
          break;
        }

        if (ResizeWidth != 0 && ResizeHeight != 0)
        {
            CleanupRenderTarget();
            SwapChain->ResizeBuffers(0, ResizeWidth, ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            ResizeWidth = ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // NOTE(antoniom): Do rendering here

        // Rendering
        ImGui::Render();
        const float ClearColorWithAlpha[4] =
        {
          ClearColor.x * ClearColor.w,
          ClearColor.y * ClearColor.w,
          ClearColor.z * ClearColor.w,
          ClearColor.w
        };

        D3DDeviceContext->OMSetRenderTargets(1, &MainRenderTargetView, NULL);
        D3DDeviceContext->ClearRenderTargetView(MainRenderTargetView, ClearColorWithAlpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        SwapChain->Present(1, 0); // Present with vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(WindowHandle);
    UnregisterClassW(WindowClass.lpszClassName, WindowClass.hInstance);

    return(0);
}

// Helper functions

bool CreateDeviceD3D(HWND WindowHandle)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC SwapChainDescription;
    ZeroMemory(&SwapChainDescription, sizeof(SwapChainDescription));

    SwapChainDescription.BufferCount                        = 2;
    SwapChainDescription.BufferDesc.Width                   = 0;
    SwapChainDescription.BufferDesc.Height                  = 0;
    SwapChainDescription.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDescription.BufferDesc.RefreshRate.Numerator   = 60;
    SwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDescription.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    SwapChainDescription.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDescription.OutputWindow                       = WindowHandle;
    SwapChainDescription.SampleDesc.Count                   = 1;
    SwapChainDescription.SampleDesc.Quality                 = 0;
    SwapChainDescription.Windowed                           = TRUE;
    SwapChainDescription.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

    UINT CreateDeviceFlags = 0;
#if defined(DEBUG)
    CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL FeatureLevel;
    const D3D_FEATURE_LEVEL FeatureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0};

    HRESULT Result = D3D11CreateDeviceAndSwapChain(NULL,
                                                   D3D_DRIVER_TYPE_HARDWARE,
                                                   NULL,
                                                   CreateDeviceFlags,
                                                   FeatureLevelArray,
                                                   2,
                                                   D3D11_SDK_VERSION,
                                                   &SwapChainDescription,
                                                   &SwapChain,
                                                   &D3DDevice,
                                                   &FeatureLevel,
                                                   &D3DDeviceContext);

    if (Result == DXGI_ERROR_UNSUPPORTED)
    {
        Result = D3D11CreateDeviceAndSwapChain(NULL,
                                               D3D_DRIVER_TYPE_WARP,
                                               NULL,
                                               CreateDeviceFlags,
                                               FeatureLevelArray,
                                               2,
                                               D3D11_SDK_VERSION,
                                               &SwapChainDescription,
                                               &SwapChain,
                                               &D3DDevice,
                                               &FeatureLevel,
                                               &D3DDeviceContext);
    }

    if (Result != S_OK)
    {
        return(false);
    }

    CreateRenderTarget();
    return(true);
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();

    if (SwapChain)
    {
      SwapChain->Release();
      SwapChain = NULL;
    }

    if (D3DDeviceContext)
    {
      D3DDeviceContext->Release();
      D3DDeviceContext = NULL;
    }

    if (D3DDevice)
    {
      D3DDevice->Release();
      D3DDevice = NULL;
    }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* BackBuffer;
    SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
    D3DDevice->CreateRenderTargetView(BackBuffer, NULL, &MainRenderTargetView);
    BackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (MainRenderTargetView)
    {
      MainRenderTargetView->Release();
      MainRenderTargetView = NULL;
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI WndProc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    if (ImGui_ImplWin32_WndProcHandler(WindowHandle, Message, WParam, LParam))
    {
        return true;
    }

    switch (Message)
    {
      case WM_SIZE:
      {
        if (WParam == SIZE_MINIMIZED)
        {
            return 0;
        }

        ResizeWidth  = (UINT) LOWORD(LParam);
        ResizeHeight = (UINT) HIWORD(LParam);

        return 0;
      }

      case WM_SYSCOMMAND:
      {
        if ((WParam & 0xfff0) == SC_KEYMENU)
        {
            return 0;
        }
      } break;

      case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 0;
      }
    }

    return DefWindowProcW(WindowHandle, Message, WParam, LParam);
}
