#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "player.hpp"
#include "block.hpp"

#include <memory>

class Ball
{
    sf::RenderWindow &m_window;

    sf::CircleShape m_body;
    float m_x;
    float m_y;
    sf::Vector2f m_speed;

    bool m_missed;
    bool m_lost;
    int m_lives;
    int m_lives_remained;

    sf::Sound &m_sound_hit, &m_sound_pop, &m_sound_crack, &m_sound_unbreakable, &m_sound_bonus_start;

    void set_speed();

public:
    Ball(sf::RenderWindow &window, float x, float y,
         sf::Sound &sound_hit, sf::Sound &sound_pop, sf::Sound &sound_crack, sf::Sound &sound_unbreakable, sf::Sound &sound_bonus_start);

    void set_pos(float x, float y);

    sf::Vector2f get_pos() const;

    float get_radius() const;

    void move(float time, Player &player, std::vector<std::unique_ptr<Block>> &blocks);

    bool is_missed() const;

    void set_missed(bool state);

    bool is_lost();

    void set_lost(bool state);

    void resume_lives();

    int get_remained_lives();

    void draw();
};