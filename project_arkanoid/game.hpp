#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "player.hpp"
#include "block.hpp"
#include "ball.hpp"
#include "star.hpp"
#include "message.hpp"
#include "button.hpp"

#include <vector>
#include <memory>
#include <string>
#include <fstream>

class Game
{
    sf::RenderWindow &m_window;

    const sf::Time m_frames_per_sec;
    sf::Time m_total_time;
    sf::Clock m_clock;

    std::vector<std::string> m_titles;
    std::vector<std::vector<std::string>> m_levels;
    size_t m_curr_level;

    Player m_player;
    std::vector<std::unique_ptr<Block>> m_blocks;
    Ball m_ball;

    sf::Music &m_music;
    sf::Sound &m_sound_game_won, &m_sound_game_lost;
    sf::Sound &m_sound_bonus_end;
    bool m_play = false;

    enum class GameState
    {
        Won,
        Lost,
        Default
    };

    GameState m_game_state = GameState::Default;

    bool m_game_started = false;
    bool m_start_moving = false;

    std::string m_text_level = "Level: ";
    Message m_message_level;

    std::string m_text_won = "Game won";
    std::string m_text_lost = "Game lost";
    Message m_message_game_state;

    std::string m_text_lives = "Lives: ";
    Message m_message_lives;

    sf::Time m_magic_time;
    void (*m_ptr_end_magic)(void *);

    // ---animation---
    std::vector<std::unique_ptr<Star>> m_stars;

    std::string m_game_name = "Arkanoid";
    Message m_message_game_name;

    std::string m_play_text = "Play";
    Button m_button;

    void load_animation();

    void update_animation();

    void run_animation();
    // ---

    bool load_levels();

    void load_blocks();

    void restart();

public:
    Game(sf::RenderWindow &window, sf::Font &font,
         sf::Music &music, sf::Sound &sound_won, sf::Sound &sound_lost,
         sf::Sound &sound_hit, sf::Sound &sound_pop, sf::Sound &sound_crack, sf::Sound &sound_unbreakable,
         sf::Sound &sound_bonus_start, sf::Sound &sound_bonus_end);

    void run();

    void increase_racket_size();

    void restore_racket_size();

    void change_blocks_type(sf::Texture &texture);

    void restore_block_type(sf::Texture &texture);

    void set_magic_duration(int time)
    {
        m_magic_time = sf::seconds(time);
    }

    void set_ptr_end_magic(void *ptr)
    {
        m_ptr_end_magic = reinterpret_cast<void (*)(void *)>(ptr);
    }

    void change_level();
};