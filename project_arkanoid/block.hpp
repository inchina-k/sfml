#pragma once

#include "SFML/Graphics.hpp"

#include <vector>

class Block
{
    void *m_game;
    sf::RenderWindow &m_window;
    sf::RectangleShape m_body;
    sf::Vector2f m_pos;
    sf::Texture m_initial_texture;
    std::vector<sf::Texture> m_damaged_textures;
    int m_initial_health;
    int m_current_health;
    bool m_is_bonus;
    void (*ptr_to_magic_start)(void *, void *);
    void (*ptr_to_magic_end)(void *);

public:
    Block(void *game, sf::RenderWindow &window,
          sf::Vector2f &size, sf::Vector2f &pos, sf::Texture &texture, int health,
          bool is_bonus, void start_magic(void *, void *), void end_magic(void *));

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

    void set_health(int health);

    void set_texture(sf::Texture &texture);

    bool is_bonus() const;

    void draw();
};