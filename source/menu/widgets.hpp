#pragma once
#include <common.hpp>

struct SItemData
{
    ID3D11ShaderResourceView* sheet;
    ImVec2 sheetsize;
};

namespace widgets
{
    bool shopitem( std::string name, std::string desc, int cost, ImVec2 size, SItemData data, ImVec2 uv );
}
