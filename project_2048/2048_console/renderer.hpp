#pragma once

#include "game_2048.hpp"

class Renderer
{
    Game &m_game;

public:
    Renderer(Game &game)
        : m_game(game)
    {
    }

    void render() const;
};