#pragma once

#include "SFML/Graphics.hpp"

#include <vector>

class Block
{
    sf::RenderWindow &m_window;
    sf::RectangleShape m_body;
    sf::Vector2f m_pos;
    sf::Texture m_initial_texture;
    std::vector<sf::Texture> m_damaged_textures;
    int m_initial_health;
    int m_health;

public:
    Block(sf::RenderWindow &window, sf::Vector2f &size, sf::Vector2f &pos, sf::Texture &texture, int health);

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

    int get_initial_health() const;

    void draw();
};