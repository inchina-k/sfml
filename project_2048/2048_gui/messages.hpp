#pragma once

#include <SFML/Graphics.hpp>
#include "game_2048.hpp"
#include "message.hpp"

#include <string>

class Messages
{
    Game &m_game;
    int m_text_size;

    std::string m_text_default = "Game 2048";
    std::string m_text_won = "Game won!";
    std::string m_text_lost = "Game lost";
    std::string m_text_goal = "Goal:\n\n";
    std::string m_text_curr_score = "Score:\n\n";
    std::string m_text_best_score = "Best:\n\n";

    std::string m_goal = m_text_goal + std::to_string(m_game.get_goal());
    std::string m_curr_score = m_text_curr_score + std::to_string(m_game.get_curr_score());
    std::string m_best_score = m_text_best_score + std::to_string(m_game.get_best_score(m_game.get_goal()));

    Message m_message_title;
    Message m_message_goal;
    Message m_message_curr_score;
    Message m_message_best_score;

public:
    Messages(Game &game, sf::Font &font, int text_size);

    void load_messages();

    void place_messages(sf::RenderWindow &window, double x, double y, int cell_size);

    void set_title_default();

    void set_title_game_won();

    void set_title_game_lost();

    void change_message_goal();

    void change_message_curr_score();

    void change_message_best_score();

    void show_messages(sf::RenderWindow &window);
};