#include "math.hpp"
#include <cstdlib>
#include <ctime>
#include <random>

int math::randomint( int x, int y )
{
    std::random_device rd;
    std::mt19937 rng{ rd( ) };
    std::uniform_int_distribution<int> uid( x, y ); // random dice
    return uid( rng );

    /*typedef std::mt19937 rng_type;
    std::uniform_int_distribution<rng_type::result_type> udist( x, y );

    rng_type rng;
    rng_type::result_type const seedval = ( unsigned )std::time( NULL );
    rng.seed( seedval );

    rng_type::result_type random_number = udist( rng );
    return random_number;*/
}
