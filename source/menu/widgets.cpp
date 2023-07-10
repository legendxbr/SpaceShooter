#include "widgets.hpp"
#include <renderer/imageloader.hpp>
#include <game/game.hpp>
#include <save system/save system.hpp>

bool widgets::shopitem( std::string label, std::string desc, int cost, ImVec2 size_arg, SItemData data, ImVec2 uv )
{
    using namespace ImGui;
    ImDrawList* list = ImGui::GetWindowDrawList( );
    ImGuiWindow* window = GetCurrentWindow( );
    if ( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label.c_str() );
    const ImVec2 label_size = CalcTextSize( label.c_str( ), NULL, true );

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if ( !ItemAdd( bb, id ) )
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, 0 );

    const ImU32 col = GetColorU32( ( held && hovered ) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button );
    RenderNavHighlight( bb, id );
    RenderFrame( bb.Min, bb.Max, col, true, style.FrameRounding );

    float rendersize = 30;
    images::renderfromsheet( data.sheet, bb.Min + ImVec2( rendersize, rendersize ), bb.Max - ImVec2( rendersize, rendersize ), uv.x, uv.y, { 80, 80 }, data.sheetsize, ImColor( 255, 255, 255, game::coins >= cost ? 255 : 100 ), list );

    std::string costlabel = "$" + std::to_string( cost );
    const ImVec2 cost_size = CalcTextSize( costlabel.c_str( ), NULL, true );

    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label.c_str( ), NULL, &label_size, { 0.5f, 0.9f }, &bb );
    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, costlabel.c_str( ), NULL, &cost_size, { 0.5f, 0.1f }, &bb );

    if ( hovered ) {
        BeginTooltip( );
        {
            ImGui::Text( desc.c_str( ) );
        }
        EndTooltip( );
    }

    if ( pressed && game::coins >= cost ) {
        game::coins -= cost;
        writesystem::save( );
    }

    return pressed;
}
