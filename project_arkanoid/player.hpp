#pragma once

#include "SFML/Graphics.hpp"

class Player
{
    sf::RenderWindow &m_window;
    sf::RectangleShape m_body;
    float m_x;
    float m_y;

public:
    Player(sf::RenderWindow &window, float x, float y);

    void set_pos(float x);

    sf::Vector2f get_pos() const;

    sf::Vector2f get_size() const;

    bool is_above(float x, float y) const;

    bool in_rect(float tx, float ty, float radius) const;

    void draw();
};