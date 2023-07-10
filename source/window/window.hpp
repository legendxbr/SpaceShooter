#pragma once
#include <common.hpp>

namespace window
{
    namespace vars
    {
        extern HWND hwnd;
        extern WNDCLASSEXA wc;
        extern UINT g_ResizeWidth, g_ResizeHeight;
    }

    bool create( std::string windowname, int w, int h );
    LRESULT WINAPI prochandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
}
