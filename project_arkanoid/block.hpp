#pragma once

#include "SFML/Graphics.hpp"

class Block
{
    sf::RenderWindow &m_window;
    sf::RectangleShape m_body;
    float m_x;
    float m_y;
    sf::Color m_initial_color;
    int m_initial_health;
    int m_health;

public:
    Block(sf::RenderWindow &window, sf::Vector2f &size, sf::Color &color, int health);

    void set_pos(float x, float y);

    sf::Vector2f get_pos() const;

    sf::Vector2f get_size() const;

    bool is_below(float x, float y) const;

    bool is_above(float x, float y) const;

    bool is_left(float x, float y) const;

    bool is_right(float x, float y) const;

    bool in_rect(float tx, float ty, float radius) const;

    bool is_ruined() const;

    void heal();

    void reduce_health();

    void draw();
};