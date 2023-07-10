#pragma once
#include <common.hpp>
#include "waves.hpp"
#include "player.hpp"
#include "asteroids.hpp"
#include "coins.hpp"

namespace game
{
    void start( );
    void tick( );
    void draw( );
    inline bool pause = false;

    inline ID3D11ShaderResourceView* sheet = nullptr;
    inline float sheetwidth = 0;
    inline float sheetheight = 0;

    inline ID3D11ShaderResourceView* coinsheet = nullptr;
    inline float coinsheetwidth = 0;
    inline float coinsheetheight = 0;

    inline int coins = 0;
    inline int shipupgrade = 0;
}
