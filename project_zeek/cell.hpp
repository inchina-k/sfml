#pragma once

#include "SFML/Graphics.hpp"

class Cell
{
    sf::RenderWindow &m_window;
    sf::RectangleShape m_body;
    sf::Vector2f m_pos;
    sf::Texture m_texture;

public:
    Cell(sf::RenderWindow &window, sf::Vector2f &size, sf::Vector2f &pos, sf::Texture &texture);

    sf::Vector2f get_pos() const;

    sf::Vector2f get_size() const;

    void draw();
};