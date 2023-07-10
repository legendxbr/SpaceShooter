#pragma once
#include <common.hpp>

namespace images
{
    bool loadfile( const char* filename, ID3D11ShaderResourceView** out_srv, float* out_width, float* out_height );
    void renderfromsheet( ID3D11ShaderResourceView* texture, ImVec2 min, ImVec2 max, int index, int indexy, ImVec2 spriteSize, ImVec2 sheetSize, ImColor color = IM_COL32_WHITE, ImDrawList* list = nullptr );
    void rotated( ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle, int index, int indexy, ImVec2 spriteSize, ImVec2 sheetSize, ImColor color = IM_COL32_WHITE );
}
