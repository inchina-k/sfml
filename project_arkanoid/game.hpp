#pragma once

#include <SFML/Graphics.hpp>

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

    Player m_player;

    std::vector<std::string> m_titles;
    std::vector<std::vector<std::string>> m_levels;

    std::vector<std::unique_ptr<Block>> m_blocks;
    Ball m_ball;

    enum class GameState
    {
        Won,
        Lost,
        Default
    };

    GameState m_game_state = GameState::Default;

    bool m_game_started = false;
    bool m_start_moving = false;

    std::string m_text_won = "Game won";
    Message m_message_game_won;

    std::string m_text_lost = "Game lost";
    Message m_message_game_lost;

    std::string m_text_lives = "Lives: ";
    Message m_message_lives;

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

    void load_blocks();

    void restart();

    bool load_levels();

public:
    Game(sf::RenderWindow &window, sf::Font &font);

    void run();
};