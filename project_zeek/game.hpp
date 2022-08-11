#pragma once

#include <SFML/Graphics.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include "player.hpp"
#include "cell.hpp"

class Game
{
    sf::RenderWindow &m_window;

    std::vector<std::string> m_titles;
    std::vector<std::vector<std::string>> m_levels;
    size_t m_curr_level;

    Player m_player;

    std::vector<std::unique_ptr<Cell>> m_cells;

    bool load_levels();
    void load_field();

public:
    Game(sf::RenderWindow &window);

    void run();
};