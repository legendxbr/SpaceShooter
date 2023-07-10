#pragma once
#include <common.hpp>

class CCoin
{
public:
    CCoin( Vector pos ) : position{pos}
    {

    }

    Vector position;
    float frameindex = 0;
};

class CCoinsController : public Singleton<CCoinsController>
{
public:
    void update( float deltaTime );
    void render( float deltaTime );
    void create( Vector position );
    void clear( ) { coins.clear( ); }
private:
    std::vector<CCoin> coins;
};
