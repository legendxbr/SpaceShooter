#include "waves.hpp"
#include "asteroids.hpp"

long delay = 30 * CLOCKS_PER_SEC;
int calculatedTime = 0;
void CWavesController::start( )
{
    now = clock( );
    wave = 1;
}

void CWavesController::update( float deltaTime, bool paused )
{
    if ( paused ) {
        now = clock( ) - calculatedTime;
        return;
    }

    delay = std::clamp( delay, 10 * CLOCKS_PER_SEC, LONG_MAX );
    calculatedTime = clock( ) - now;
    if ( clock( ) - now >= delay ) {
        now = clock( );
        wave++;

        if ( wave % 5 == 0 ) {
            CAsteroidsController::get( ).spawnReset -= 1;
        }
        else if ( wave % 2 == 0 ) {
            delay -= 2 * CLOCKS_PER_SEC;
        }
    }
}

void CWavesController::render( float deltaTime )
{
    int time = ( delay - calculatedTime ) / CLOCKS_PER_SEC;
    std::string wave = std::to_string( time );
    ImVec2 wavesize = ImGui::CalcTextSize( wave.c_str( ) );
    ImGui::GetForegroundDrawList( )->AddText( { 300 - wavesize.x * 0.5f, 30 }, ImColor( 235, 235, 52 ), wave.c_str( ) );
}
