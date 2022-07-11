#pragma once

#include "SFML/Graphics.hpp"
#include "player.hpp"
#include "block.hpp"

class Ball
{
    sf::RenderWindow &m_window;
    sf::CircleShape m_body;
    float m_x;
    float m_y;
    sf::Vector2f m_speed;

    void set_random_speed();

    bool right_collision(Block &block, float &x_coord);

    bool left_collision(Block &block, float &x_coord);

    bool top_collision(Block &block, float &y_coord);

    bool bottom_collision(Player &player, Block &block, float &y_coord);

public:
    Ball(sf::RenderWindow &window, float x, float y);

    void move(float time, Player &player, Block &block);

    void draw();
};