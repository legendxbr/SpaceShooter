#pragma once
#include <common.hpp>

namespace device
{
    namespace vars
    {
        extern ID3D11Device*            g_pd3dDevice;
        extern ID3D11DeviceContext*     g_pd3dDeviceContext;
        extern IDXGISwapChain*          g_pSwapChain;
        extern ID3D11RenderTargetView*  g_mainRenderTargetView;
    }

    bool createdevice( HWND hWnd );
    void cleanupdevice( );
    void createrendertarget( );
    void cleanuprendertarget( );
}
