#include "game.hpp"
#include "player.hpp"
#include "asteroids.hpp"
#include "waves.hpp"
#include <menu/menu.hpp>
#include <renderer/imageloader.hpp>
#include "coins.hpp"

void game::start( )
{
    if ( sheet == nullptr ) {
        images::loadfile( SPRITES_PATH "shipSheet.png", &sheet, &sheetwidth, &sheetheight );
        images::loadfile( SPRITES_PATH "coin.png", &coinsheet, &coinsheetwidth, &coinsheetheight );
    }
}

void game::tick( )
{
    float deltaTime = ImGui::GetIO( ).DeltaTime;
    if ( ImGui::IsKeyReleased( ImGuiKey_Escape ) ) {
        pause = !pause;
    }

    CWavesController::get( ).update( deltaTime, pause );
    if ( !pause ) {
        CCoinsController::get( ).update( deltaTime );
        CAsteroidsController::get( ).update( deltaTime );

        if ( playerInstance ) {
            playerInstance->update( deltaTime );
        }
    }
    else {
        const char* paused = "paused";
        ImVec2 size = ImGui::CalcTextSize( paused );
        ImGui::GetForegroundDrawList( )->AddText( { 300 - size.x * 0.5f, 300 - size.y * 0.5f }, ImColor( 235, 235, 52 ), paused );
    }
}

void game::draw( )
{
    float deltaTime = ImGui::GetIO( ).DeltaTime;
    CCoinsController::get( ).render( deltaTime );
    CAsteroidsController::get( ).render( deltaTime );
    if ( playerInstance ) {
        playerInstance->render( deltaTime );
    }
    
    if ( menu::state == GAMESTATE_GAME ) {
        CWavesController::get( ).render( deltaTime );
        std::string wave = "wave " + std::to_string( CWavesController::get( ).wave );
        ImVec2 wavesize = ImGui::CalcTextSize( wave.c_str( ) );
        ImGui::GetForegroundDrawList( )->AddText( { 10, 10 }, ImColor( 235, 235, 52 ), std::to_string( ( int )ImGui::GetIO( ).Framerate ).c_str( ) );
        ImGui::GetForegroundDrawList( )->AddText( { 300 - wavesize.x * 0.5f, 10 }, ImColor( 235, 235, 52 ), wave.c_str( ) );
    }

    std::string coin = std::to_string( coins );
    ImVec2 pointsize = ImGui::CalcTextSize( coin.c_str( ) );
    ImGui::GetForegroundDrawList( )->AddText( { 600 - pointsize.x - 25, 10 }, ImColor( 235, 235, 52 ), coin.c_str( ) );
}
