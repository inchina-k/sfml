#pragma once

#include "SFML/Graphics.hpp"

#include <vector>

class Star
{
    sf::CircleShape m_body;
    float m_radius;
    sf::Vector2f m_pos;
    sf::Vector2f m_speed;

    std::vector<sf::Color> m_colors = {sf::Color::White, sf::Color::Cyan, sf::Color::Magenta, sf::Color::Blue, sf::Color::Green};

    void load(sf::RenderWindow &window);

public:
    Star(sf::RenderWindow &window);

    void update(sf::RenderWindow &window);

    void draw(sf::RenderWindow &window);
};