#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

#include "json.hpp"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include <Windows.h>
#include <imgui/imgui_internal.h>
#include <vector>

#include <math/math.hpp>
#include <math/singleton.hpp>

using namespace nlohmann;

#define SPRITES_PATH "sprites\\"
#define FONTS_PATH "fonts\\"
inline bool openDebug = false;
