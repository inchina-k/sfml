#pragma once

#include "SFML/Graphics.hpp"

class Snowflake
{
    const float M_MIN_SZ = 30;
    const float M_MAX_SZ = 100;
    const float M_MIN_SPEED = 1;

    float m_size;
    float m_x, m_y;
    float m_dx, m_dy;
    float m_angle;
    float m_da;
    float m_rnd;
    sf::Color m_color;

    sf::RectangleShape line_1;
    sf::RectangleShape line_2;
    sf::RectangleShape line_3;
    sf::RectangleShape line_4;

    void set_pos();

public:
    Snowflake(sf::RenderWindow &window);

    void setup(sf::RenderWindow &window);

    void draw(sf::RenderWindow &window);
};