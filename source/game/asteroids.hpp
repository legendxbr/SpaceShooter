#pragma once
#include <common.hpp>
#include <vector>

class CAsteroid
{
public:
    CAsteroid( );

    int health = 0;
    int spriteindex = 0;
    float angle = 0;
    ImColor color = ImColor( 255, 255, 255 );
    ImVec2 size;
    Vector position;
};

class CAsteroidsController : public Singleton<CAsteroidsController>
{
public:
    CAsteroidsController( )
    {

    }

    void update( float deltaTime );
    void render( float deltaTime );
    void collision( );
    void debug( );
public:
    std::vector<CAsteroid> asteroids;
    float spawnReset = 8;
private:
    float spawnTime = 0;
    float spawnDelay = 8;
};
