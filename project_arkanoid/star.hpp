#pragma once

#include "SFML/Graphics.hpp"

#include <vector>

class Star
{
    sf::CircleShape m_body;
    float m_radius;
    float m_x;
    float m_y;
    float m_z;

    std::vector<sf::Color> m_colors = {sf::Color::White, sf::Color::Cyan, sf::Color::Magenta, sf::Color::Blue, sf::Color::Green};

public:
    Star(sf::RenderWindow &window);

    void update(sf::RenderWindow &window);

    void draw(sf::RenderWindow &window);
};