#pragma once
#include <cmath>

struct Vector
{
    Vector( ) = default;
    Vector( float x, float y, float z ) : x{ x }, y{ y }, z{ z } {}
    Vector( float x, float y ) : x{ x }, y{ y } {}

    bool operator==( const Vector& v )
    {
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=( const Vector& v )
    {
        return !( *this == v );
    }

    Vector& operator+=( const Vector& v )
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector& operator+=( float f )
    {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    Vector& operator-=( const Vector& v )
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector& operator-=( float f )
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    Vector operator-( const Vector& v )
    {
        return Vector{ x - v.x, y - v.y, z - v.z };
    }

    Vector operator+( const Vector& v )
    {
        return Vector{ x + v.x, y + v.y, z + v.z };
    }

    Vector operator*( const Vector& v )
    {
        return Vector{ x * v.x, y * v.y, z * v.z };
    }

    Vector& operator/=( float div )
    {
        x /= div;
        y /= div;
        z /= div;
        return *this;
    }

    Vector operator*( float mul )
    {
        return Vector{ x * mul, y * mul, z * mul };
    }

    Vector operator-( float sub )
    {
        return Vector{ x - sub, y - sub, z - sub };
    }

    Vector operator+( float add )
    {
        return Vector{ x + add, y + add, z + add };
    }

    float length( )
    {
        return std::sqrt( x * x + y * y + z * z );
    }

    float squareLength( )
    {
        return x * x + y * y + z * z;
    }

    float dotProduct( const Vector& v )
    {
        return x * v.x + y * v.y + z * v.z;
    }

    float distTo( const Vector& v )
    {
        return ( *this - v ).length( );
    }

    float distToSqr( const Vector& v )
    {
        return ( *this - v ).squareLength( );
    }

    float x = 0, y = 0, z = 0;
};

namespace math
{
    int randomint( int x, int y );
}
