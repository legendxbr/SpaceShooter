#include "menu.hpp"
#include <game/game.hpp>
#include <save system/save system.hpp>
#include <menu/widgets.hpp>

void menu::start( )
{
    ImGuiIO& io = ImGui::GetIO( );
    title = io.Fonts->AddFontFromFileTTF( FONTS_PATH "nasalization.otf", 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic( ) );
    writesystem::load( );
}

void menu::draw( )
{
    std::vector<CShopData> ships =
    {
        { 100, "ship upgrade 1", "-double shoot\n-less speed", {1, 0} },
        { 300, "ship upgrade 2", "-triple shoot\n-extra damage\n-less speed", {2, 0} },
        { 700, "ship upgrade 3", "-triple shoot\n-extra damage\n-explosive bullets\n-more speed", {3, 0} }
    };

    static ImVec2 size = ImVec2( 300, 300 );

    if ( state == GAMESTATE_SHOP ) {
        ImGui::SetNextWindowPos( { 300 - size.x * 0.5f, 10 }, ImGuiCond_Always );
        ImGui::SetNextWindowSize( {100, 30}, ImGuiCond_Always );
        ImGui::Begin( "###topmenu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar );
        {
            ImGui::SetCursorPosY( 5 );
            if ( ImGui::Button( "return" ) ) {
                state = GAMESTATE_MENU;
            }
        }
        ImGui::End( );
    }

    ImGui::SetNextWindowPos( { 300 - size.x * 0.5f, 300 - size.y * 0.5f }, ImGuiCond_Always );
    ImGui::SetNextWindowSize( size, ImGuiCond_Always );
    ImGui::Begin( "###name", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );
    {
        ImVec2 pos = ImGui::GetWindowPos( );
        ImDrawList* list = ImGui::GetWindowDrawList( );

        if ( state == GAMESTATE_MENU || state == GAMESTATE_DEATH ) {
            ImVec2 titlesize = title->CalcTextSizeA( title->FontSize, FLT_MAX, 0, "spacegui" );
            list->AddText( title, title->FontSize, pos + ImVec2( size.x * 0.5f - titlesize.x * 0.5f, 20 ), ImColor( 255, 255, 255 ),
                "spacegui" );

            size = ImVec2( 300, 300 );
            ImGui::SetCursorPos( { 110, 90 } );
            if ( ImGui::Button( "start", { 80, 30 } ) ) {
                playerInstance = new CPlayer( );
                CWavesController::get( ).start( );
                CAsteroidsController::get( ).asteroids.clear( );
                CCoinsController::get( ).clear( );

                writesystem::save( );
                state = GAMESTATE_GAME;
            }
            ImGui::SetCursorPosX( 110 );
            if(ImGui::Button( "shop", { 80, 30 } )) {
                state = GAMESTATE_SHOP;
            }
            ImGui::SetCursorPosX( 110 );
            if ( ImGui::Button( "quit", { 80, 30 } ) ) {
                exit( 0 );
            }
        }
        else if ( state == GAMESTATE_SHOP ) {
            size = ImVec2( 500, 500 );

            SItemData data;
            data.sheet = game::sheet;
            data.sheetsize = { game::sheetwidth, game::sheetheight };

            float spacing = 17;
            for ( int i = 0; i < ships.size( ); i++ ) {
                if ( game::shipupgrade == i ) {
                    if ( widgets::shopitem( ships[ i ].name, ships[ i ].desc, ships[ i ].cost,
                        { 150, 150 }, data, ships[ i ].spriteindex ) ) {
                        game::shipupgrade = i + 1;

                        if ( game::shipupgrade >= ships.size( ) - 1 ) {
                            ships.clear( );
                        }

                        break;
                    }
                }
            }
        }
    }
    ImGui::End( );
}
