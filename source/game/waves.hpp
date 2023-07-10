#pragma once
#include <common.hpp>
#include <cstdio>
#include <iostream>
#include <ctime>

class CWavesController : public Singleton<CWavesController>
{
public:
    void start( );
    void update( float deltaTime, bool paused );
    void render( float deltaTime );
public:
    int wave = 1;
    clock_t now;
};
