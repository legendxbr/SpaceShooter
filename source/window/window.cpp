#include "window.hpp"
#include "device.hpp"

using namespace window::vars;
bool window::create( std::string windowname, int w, int h )
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow( );
    GetWindowRect( hDesktop, &desktop );

    float x = desktop.right * 0.5f - w * 0.5f;
    float y = desktop.bottom * 0.5f - h * 0.5f;

    wc = { sizeof( wc ), CS_CLASSDC, prochandler, 0L, 0L, GetModuleHandle( nullptr ), nullptr, nullptr, nullptr, nullptr, "ImGui Example", nullptr };
    RegisterClassExA( &wc );
    hwnd = CreateWindowExA( 0, wc.lpszClassName, windowname.c_str( ), WS_OVERLAPPEDWINDOW, x, y, w, h, nullptr, nullptr, wc.hInstance, nullptr );

    if ( !device::createdevice( hwnd ) ) {
        device::cleanupdevice( );
        UnregisterClassA( wc.lpszClassName, wc.hInstance );
        return false;
    }

    ShowWindow( hwnd, SW_SHOWDEFAULT );
    UpdateWindow( hwnd );

    if ( device::vars::g_pd3dDevice == nullptr )
        return false;

    return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT WINAPI window::prochandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
        return true;

    switch ( msg ) {
    case WM_SIZE:
        if ( wParam == SIZE_MINIMIZED )
            return 0;
        g_ResizeWidth = ( UINT )LOWORD( lParam ); // Queue resize
        g_ResizeHeight = ( UINT )HIWORD( lParam );
        return 0;
    case WM_SYSCOMMAND:
        if ( ( wParam & 0xfff0 ) == SC_KEYMENU ) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage( 0 );
        return 0;
    }
    return ::DefWindowProcA( hWnd, msg, wParam, lParam );
}

namespace window
{
    namespace vars
    {
        HWND hwnd;
        WNDCLASSEXA wc;
        UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
    }
}
