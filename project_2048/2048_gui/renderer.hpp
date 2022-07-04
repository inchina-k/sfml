#pragma once

#include <SFML/Graphics.hpp>
#include "game_2048.hpp"

#include <unordered_map>

class Renderer
{
    Game &m_game;
    sf::Texture m_texture;
    std::unordered_map<int, sf::Sprite> m_images;
    int m_x;
    int m_y;
    int m_num_of_cells;
    double m_cell_size;
    int m_image_size;

public:
    Renderer(Game &game, sf::Texture &texture, int num_of_cells, int x, int y, double cell_size, int image_size);

    void set_texture(sf::Texture &texture);

    void render(sf::RenderWindow &window);

    void restart_game(sf::RenderWindow &window);
};