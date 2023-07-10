#pragma once
#include <common.hpp>
#include <vector>

class CBullet
{
public:
    CBullet( Vector pos ) : position{pos }{ }
    Vector position;
    float damage = 5;
};

class CPlayer
{
public:
    CPlayer( );

    struct CPlayerStats
    {
        int health = 100;
    };

    void start( );
    void update( float deltaTime );
    void keyboard( float deltaTime );

    void render( float deltaTime );
    void debug( float deltaTime );

public:
    CPlayerStats stats;
    ImColor color = IM_COL32_WHITE;
    Vector position = Vector( 0, 0, 0 );
    std::vector<CBullet> bullets;
private:
    float bulletTime;
    float bulletReset = 2;
    float bulletDelay = 8;
};

inline CPlayer* playerInstance = nullptr;
