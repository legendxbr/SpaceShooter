#include "player.hpp"
#include "asteroids.hpp"
#include "game.hpp"
#include <algorithm>
#include <renderer/imageloader.hpp>
#include <math/collision.hpp>

CPlayer::CPlayer( )
{
    start( );
}

void CPlayer::start( )
{
    position = Vector( 300 - 40, 470 );
}

void CPlayer::update( float deltaTime )
{
    auto it = bullets.begin( );
    while ( it != bullets.end( ) ) {
        it->position.y -= 190 * deltaTime;
        
        bool collided = false;
        auto asteroids = CAsteroidsController::get( ).asteroids.begin( );
        while ( asteroids != CAsteroidsController::get( ).asteroids.end( ) ) {
            if ( asteroids->position.distTo( it->position ) > 100 ) {
                asteroids++;
                continue;
            }

            float cos_a = cosf( asteroids->angle );
            float sin_a = sinf( asteroids->angle );

            ImVec2 center = ImVec2( asteroids->position.x, asteroids->position.y );
            ImVec2 size = asteroids->size;
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

            ImVec2 bulletpos = ImVec2( it->position.x, it->position.y );
            
            for ( int i = 0; i < pos.size( ); i++ ) {
                if ( i < pos.size( ) - 1 ) {
                    collided = collision::intersectline( pos[ i ], pos[ i + 1 ], bulletpos - ImVec2( 2, 2 ), bulletpos - ImVec2( 2, -2 ) ) ||
                        collision::intersectline( pos[ i ], pos[ i + 1 ], bulletpos + ImVec2( 2, 2 ), bulletpos + ImVec2( 2, -2 ) ) ||
                        collision::intersectline( pos[ i ], pos[ i + 1 ], bulletpos - ImVec2( 2, 2 ), bulletpos - ImVec2( -2, 2 ) ) ||
                        collision::intersectline( pos[ i ], pos[ i + 1 ], bulletpos + ImVec2( 2, 2 ), bulletpos + ImVec2( -2, 2 ) );

                    if ( collided )
                        break;
                }
            }

            if ( collided ) {
                asteroids->health -= it->damage;
                asteroids->color = ImColor( 255, 0, 0 );
                it = bullets.erase( it );
                break;
            }
            else {
                ++asteroids;
            }
        }

        if ( collided ) {
            continue;
        }

        if ( it->position.y < 10 ) {
            it = bullets.erase( it );
        }
        else {
            ++it;
        }
    }

    color = ImLerp( color, ImColor( 255, 255, 255 ), deltaTime * 5 );
    stats.health = std::clamp( stats.health, 0, 100 );
    keyboard( deltaTime );
}

void CPlayer::render( float deltaTime )
{
    images::renderfromsheet( game::sheet, { position.x - 40, position.y - 40 }, { position.x + 40, position.y + 40 }, game::shipupgrade, 0, { 80,80 }, { game::sheetwidth, game::sheetheight }, color );

    for ( auto& bullet : bullets ) {
        ImVec2 pos = ImVec2( bullet.position.x, bullet.position.y );
        ImGui::GetBackgroundDrawList( )->AddRectFilled( pos - ImVec2( 2, 2 ), pos + ImVec2( 2, 2 ), ImColor( 235, 235, 52 ) );
    }

    debug( deltaTime );
}

void CPlayer::keyboard( float deltaTime )
{
    std::vector<float> speeds = {
        150,
        112.5f,
        75,
        170
    };

    float speed = speeds[ game::shipupgrade ];
    if ( ImGui::IsKeyDown( ImGuiKey_A ) && position.x >= 40 ) {
        position.x -= speed * deltaTime;
    }
    else if ( ImGui::IsKeyDown( ImGuiKey_D ) && position.x <= 540 ) {
        position.x += speed * deltaTime;
    }

    if ( ImGui::IsKeyDown( ImGuiKey_W ) && position.y >= 40 ) {
        position.y -= speed * deltaTime;
    }
    else if ( ImGui::IsKeyDown( ImGuiKey_S ) && position.y <= 540 ) {
        position.y += speed * deltaTime;
    }

    bulletTime -= bulletDelay * deltaTime;
    if ( ImGui::IsKeyPressed( ImGuiKey_Space ) && bulletTime <= 0 ) {
        if ( game::shipupgrade == 0 ) {
            bullets.push_back( CBullet( position - Vector( 0, 35 ) ) );
        }
        else if ( game::shipupgrade == 1 ) {
            bullets.push_back( CBullet( position - Vector( 32, 25 ) ) );
            bullets.push_back( CBullet( position - Vector( -32, 25 ) ) );
        }
        else if ( game::shipupgrade == 2 ) {
            bullets.push_back( CBullet( position - Vector( 0, 35 ) ) );
            bullets.push_back( CBullet( position - Vector( 13, 30 ) ) );
            bullets.push_back( CBullet( position - Vector( -13, 30 ) ) );
        }
        
        //bullets.push_back( CBullet( position - Vector( 15, 35 ) ) );
        //bullets.push_back( CBullet( position - Vector( -15, 35 ) ) );
        bulletTime = bulletReset;
    }
}

void CPlayer::debug( float deltaTime )
{
    if ( ImGui::IsKeyReleased( ImGuiKey_Insert ) ) {
        openDebug = !openDebug;
    }

    if ( !openDebug ) {
        return;
    }

    Vector playerPosition = playerInstance->position;
    ImVec2 playerVec = ImVec2( playerPosition.x, playerPosition.y );
    ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( playerVec.x - 37, playerVec.y - 10 ), ImVec2( playerVec.x - 37, playerVec.y + 30 ), ImColor( 255, 255, 255 ) );
    ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( playerVec.x + 36, playerVec.y - 10 ), ImVec2( playerVec.x + 36, playerVec.y + 30 ), ImColor( 255, 255, 255 ) );
    ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( playerVec.x - 37, playerVec.y + 30 ), ImVec2( playerVec.x + 36, playerVec.y + 30 ), ImColor( 255, 255, 255 ) );
    ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( playerVec.x - 37, playerVec.y - 10 ), ImVec2( playerVec.x, playerVec.y - 37 ), ImColor( 255, 255, 255 ) );
    ImGui::GetBackgroundDrawList( )->AddLine( ImVec2( playerVec.x + 36, playerVec.y - 10 ), ImVec2( playerVec.x, playerVec.y - 37 ), ImColor( 255, 255, 255 ) );

    ImGui::GetBackgroundDrawList( )->AddRectFilled( ImVec2( playerVec.x - 40, playerVec.y + 35 ), ImVec2( playerVec.x - 40 + ( stats.health / 100.f ) * 80, playerVec.y + 40 ), ImColor( 255, 0, 0 ) );
    ImGui::GetBackgroundDrawList( )->AddText( ImVec2( playerVec.x - 40, playerVec.y + 45 ), ImColor( 255, 255, 255 ), std::to_string( ( int )stats.health ).c_str( ) );

    ImGui::Begin( "debug" );
    {
        ImGui::Text( "%f", deltaTime );
        ImGui::Text( "%d", bullets.size() );
        ImGui::Text( "%f", bulletTime );
    }
    ImGui::End( );
}
