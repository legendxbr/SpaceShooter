#include "save system.hpp"
#include "player.hpp"
#include "game.hpp"
#include <fstream>

void writesystem::load( )
{
    std::ifstream filedata( "data.txt" );
    if ( filedata.is_open( ) ) {
        std::string read;
        filedata >> read;
        json data = json::parse( read );
        game::coins = data[ "coins" ].get<int>( );
        game::shipupgrade = data[ "ship" ].get<int>( );
    }
}

void writesystem::save( )
{
    json data;
    data[ "coins" ] = game::coins;
    data[ "ship" ] = game::shipupgrade;

    std::fstream f( "data.txt", std::ios::out );
    f << data.dump( );
    f.close( );
}
