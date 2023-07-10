#include "collision.hpp"

bool collision::intersectline( ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4 )
{
    float x1 = p1.x;
    float y1 = p1.y;
    float x2 = p2.x;
    float y2 = p2.y;
    float x3 = p3.x;
    float y3 = p3.y;
    float x4 = p4.x;
    float y4 = p4.y;

    float uA = ( ( x4 - x3 )*( y1 - y3 ) - ( y4 - y3 )*( x1 - x3 ) ) / ( ( y4 - y3 )*( x2 - x1 ) - ( x4 - x3 )*( y2 - y1 ) );
    float uB = ( ( x2 - x1 )*( y1 - y3 ) - ( y2 - y1 )*( x1 - x3 ) ) / ( ( y4 - y3 )*( x2 - x1 ) - ( x4 - x3 )*( y2 - y1 ) );

    // if uA and uB are between 0-1, lines are colliding
    if ( uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1 ) {

        // optionally, draw a circle where the lines meet
        float intersectionX = x1 + ( uA * ( x2 - x1 ) );
        float intersectionY = y1 + ( uA * ( y2 - y1 ) );
        return true;
    }

    return false;
}
