#pragma once

#include "SFML/Graphics.hpp"

class Block
{
    sf::RenderWindow &m_window;
    sf::RectangleShape m_body;
    float m_x;
    float m_y;

public:
    Block(sf::RenderWindow &window, float x, float y);

    sf::Vector2f get_pos() const;

    sf::Vector2f get_size() const;

    void draw();
};