#include "coins.hpp"
#include "game.hpp"
#include "player.hpp"
#include <renderer/imageloader.hpp>

void CCoinsController::update( float deltaTime )
{
    auto it = coins.begin( );
    while ( it != coins.end( ) ) {
        it->frameindex += 7 * deltaTime;
        if ( it->frameindex > 7 )
            it->frameindex = 0;

        if ( it->position.distTo( playerInstance->position ) < 30 ) {
            game::coins += math::randomint( 1, 5 );
            it = coins.erase( it );
        }
        else {
            it++;
        }
    }
}

void CCoinsController::render( float deltaTime )
{
    for ( auto& coin : coins ) {
        Vector position = coin.position;
        ImVec2 size = ImVec2( 40, 40 );
        images::renderfromsheet( game::coinsheet,
            { position.x - size.x * 0.5f, position.y - size.x * 0.5f },
            { position.x + size.x * 0.5f, position.y + size.x * 0.5f },
            coin.frameindex, 0, {80, 80}, { game::coinsheetwidth, game::coinsheetheight } );
    }
}

void CCoinsController::create( Vector position )
{
    coins.push_back( CCoin( position ) );
}
