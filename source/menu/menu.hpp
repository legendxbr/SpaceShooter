#pragma once
#include <common.hpp>

enum GameStates : int
{
    GAMESTATE_MENU,
    GAMESTATE_SHOP,
    GAMESTATE_GAME,
    GAMESTATE_DEATH
};

struct CShopData
{
    int cost;
    std::string name;
    std::string desc;
    ImVec2 spriteindex;
};

namespace menu
{
    void start( );
    void draw( );

    inline int state = GAMESTATE_MENU;
    inline ImFont* title = nullptr;
}
