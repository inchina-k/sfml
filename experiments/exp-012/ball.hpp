#pragma once

#include "SFML/Graphics.hpp"
#include "player.hpp"
#include "block.hpp"

enum class States
{
    SetPos,
    SetDir,
    Move
};

class Ball
{
    sf::RenderWindow &m_window;
    sf::CircleShape m_body;
    float m_x;
    float m_y;
    sf::Vector2f m_speed;

    States m_state = States::SetPos;

    void set_random_speed();

    bool right_collision(Block &block, float &x_coord);

    bool left_collision(Block &block, float &x_coord);

    bool top_collision(Block &block, float &y_coord);

    bool bottom_collision(Player &player, Block &block, float &y_coord);

public:
    Ball(sf::RenderWindow &window, float x, float y);

    void set_pos(float x, float y);

    sf::Vector2f get_pos() const;

    States get_state() const;

    void set_state(States state);

    void set_speed(sf::Vector2f speed);

    void move(float time, Player &player, Block &block);

    void draw();
};