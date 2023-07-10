#include "asteroids.hpp"
#include "player.hpp"
#include "game.hpp"
#include "coins.hpp"
#include <algorithm>
#include <save system/save system.hpp>
#include <renderer/imageloader.hpp>
#include <math/collision.hpp>
#include <menu/menu.hpp>

void CAsteroidsController::update( float deltaTime )
{
    spawnTime -= spawnDelay * deltaTime;
    if ( spawnTime <= 0 ) {
        spawnTime = spawnReset;
        asteroids.push_back( CAsteroid( ) );
    }

    auto it = asteroids.begin( );
    while ( it != asteroids.end( ) ) {
        it->position.y += 150 * deltaTime;
        it->color = ImLerp( it->color, ImColor( 255, 255, 255 ), deltaTime * 5 );
        it->angle += deltaTime * 1.0f;

        if ( it->position.y > 630 || it->health <= 0 ) {
            if ( it->health <= 0 ) {
                int chance = math::randomint( 0, 100 );
                if ( chance <= 30 ) {
                    CCoinsController::get( ).create( it->position );
                }
            }

            it = asteroids.erase( it );
        }
        else {
            ++it;
        }
    }

    collision( );
}

void CAsteroidsController::render( float deltaTime )
{
    std::vector<ImVec2> collisionSizes =
    {
        { 45, 45 },
        { 55, 55 },
        { 60, 60 },
        { 70, 70 },
        { 75, 75 }
    };

    for ( auto& asteroid : asteroids ) {
        images::rotated( game::sheet, { asteroid.position.x, asteroid.position.y },
            { 80, 80 }, asteroid.angle,
            asteroid.spriteindex, 1, { 80,80 }, { game::sheetwidth, game::sheetheight },
            asteroid.color );
    }

    debug( );
}

void CAsteroidsController::collision( )
{
    Vector playerPosition = playerInstance->position;
    ImVec2 playerVec = ImVec2( playerPosition.x, playerPosition.y );

    auto it = asteroids.begin( );
    while ( it != asteroids.end( ) ) {
        float cos_a = cosf( it->angle );
        float sin_a = sinf( it->angle );

        if ( it->position.distTo( playerInstance->position ) > 100 ) {
            it++;
            continue;
        }

        ImVec2 center = ImVec2( it->position.x, it->position.y );
        ImVec2 size = it->size;
        std::vector<ImVec2> pos =
        {
            center + ImRotate( ImVec2( -size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( -size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),

            center + ImRotate( ImVec2( size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( -size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( -size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),
        };

        bool collided = false;
        for ( int i = 0; i < pos.size( ); i++ ) {
            if ( i < pos.size( ) - 1 ) {
                collided = collision::intersectline( pos[ i ], pos[ i + 1 ], ImVec2( playerVec.x - 37, playerVec.y - 10 ), ImVec2( playerVec.x - 37, playerVec.y + 30 ) ) ||
                    collision::intersectline( pos[ i ], pos[ i + 1 ], ImVec2( playerVec.x + 36, playerVec.y - 10 ), ImVec2( playerVec.x + 36, playerVec.y + 30 ) ) ||
                    collision::intersectline( pos[ i ], pos[ i + 1 ], ImVec2( playerVec.x - 37, playerVec.y + 30 ), ImVec2( playerVec.x + 36, playerVec.y + 30 ) ) ||
                    collision::intersectline( pos[ i ], pos[ i + 1 ], ImVec2( playerVec.x - 37, playerVec.y - 10 ), ImVec2( playerVec.x, playerVec.y - 37 ) ) ||
                    collision::intersectline( pos[ i ], pos[ i + 1 ], ImVec2( playerVec.x + 36, playerVec.y - 10 ), ImVec2( playerVec.x, playerVec.y - 37 ) );

                if ( collided )
                    break;
            }
        }

        if ( collided ) {
            playerInstance->color = ImColor( 255, 0, 0 );
            playerInstance->stats.health -= 10 * ( std::clamp( it->spriteindex, 1, 4 ) );
            if ( playerInstance->stats.health <= 0 ) {
                playerInstance = nullptr;
                menu::state = GAMESTATE_DEATH;
                writesystem::save( );
                break;
            }

            it = asteroids.erase( it );
        }
        else {
            ++it;
        }
    }
}

void CAsteroidsController::debug( )
{
    if ( !openDebug )
        return;

    for ( auto& asteroid : asteroids ) {
        float cos_a = cosf( asteroid.angle );
        float sin_a = sinf( asteroid.angle );

        ImVec2 center = ImVec2( asteroid.position.x, asteroid.position.y );

        if ( playerInstance ) {
            ImGui::GetForegroundDrawList( )->AddText( center, ImColor( 255, 255, 0 ), std::to_string( ( int )asteroid.position.distTo( playerInstance->position ) ).c_str( ) );
        }
        
        ImVec2 size = asteroid.size;
        std::vector<ImVec2> pos =
        {
            /*side*/
            center + ImRotate( ImVec2( -size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( -size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),

            center + ImRotate( ImVec2( size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( -size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
            center + ImRotate( ImVec2( -size.x * 0.5f, size.y * 0.5f ), cos_a, sin_a ),
        };

        for ( int i = 0; i < pos.size( ); i++ ) {
            if ( i < pos.size( ) - 1 ) {
                ImGui::GetForegroundDrawList( )->AddLine( pos[ i ], pos[ i + 1 ], ImColor( 255, 255, 255 ) );
            }
        }
    }
}

CAsteroid::CAsteroid( )
{
    static int lastGenerated = -1;
    std::vector<ImVec2> collisionSizes =
    {
        { 45, 45 },
        { 55, 55 },
        { 60, 60 },
        { 70, 70 },
        { 75, 75 }
    };

    std::vector<int> healths =
    {
        10,
        15,
        20,
        25,
        30
    };

    this->position.x = math::randomint( 40, 560 );
    this->spriteindex = math::randomint( 0, 4 );
    this->size = collisionSizes[ this->spriteindex ];
    this->health = healths[ this->spriteindex ];

    if ( lastGenerated >= 0 ) {
        if ( this->position.x > 300 && lastGenerated > 300 ) {
            this->position.x = math::randomint( 40, 400 );
        }
        else if ( this->position.x < 300 && lastGenerated < 300 ) {
            this->position.x = math::randomint( 200, 560 );
        }
    }

    lastGenerated = this->position.x;
}
