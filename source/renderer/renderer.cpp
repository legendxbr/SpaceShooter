#include "renderer.hpp"
#include "imageloader.hpp"
#include <game/game.hpp>
#include <menu/menu.hpp>
#include <window/window.hpp>
#include <window/device.hpp>

using namespace window::vars;
using namespace device::vars;

bool renderer::start( )
{
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( ); ( void )io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark( );

    if ( !ImGui_ImplWin32_Init( hwnd ) )
        return false;

    return ImGui_ImplDX11_Init( g_pd3dDevice, g_pd3dDeviceContext );
}

void renderer::update( bool vsync )
{
    game::start( );
    menu::start( );

    float tmpw, tmph;
    ID3D11ShaderResourceView* background;
    images::loadfile( SPRITES_PATH "space.png", &background, &tmpw, &tmph );

    ImVec4 clear_color = ImVec4( 0.f, 0.f, 0.f, 1.00f );
    bool done = false;
    while ( !done ) {
        MSG msg;
        while ( ::PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) ) {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
            if ( msg.message == WM_QUIT )
                done = true;
        }

        if ( done )
            break;

        if ( g_ResizeWidth != 0 && g_ResizeHeight != 0 ) {
            device::cleanuprendertarget( );
            g_pSwapChain->ResizeBuffers( 0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0 );
            g_ResizeWidth = g_ResizeHeight = 0;
            device::createrendertarget( );
        }

        ImGui_ImplDX11_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        ImGui::GetBackgroundDrawList( )->AddImage( background, { 0,0 }, { 600,600 } );

        game::draw( );
        if ( menu::state == GAMESTATE_GAME ) {
            game::tick( );
        }
        else {
            menu::draw( );
        }

        ImGui::Render( );
        const float clear_color_with_alpha[ 4 ] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets( 1, &g_mainRenderTargetView, nullptr );
        g_pd3dDeviceContext->ClearRenderTargetView( g_mainRenderTargetView, clear_color_with_alpha );
        ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

        g_pSwapChain->Present( vsync, 0 );
    }
}

void renderer::shutdown( )
{
    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    device::cleanupdevice( );
    DestroyWindow( hwnd );
    UnregisterClassA( wc.lpszClassName, wc.hInstance );
}
