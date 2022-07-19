#include "game.hpp"

#include "../libs/random.hpp"

#include <sstream>
#include <iostream>
#include <unordered_map>

using Random = effolkronium::random_static;

Game::Game(sf::RenderWindow &window, sf::Font &font)
    : m_window(window), m_frames_per_sec(sf::seconds(0.01f)), m_total_time(sf::Time::Zero), m_curr_level(0),
      m_player(m_window, m_window.getSize().x / 2, m_window.getSize().y - m_player.get_size().y - 30),
      m_ball(m_window, m_window.getSize().x / 2, m_player.get_pos().y - m_player.get_size().y * 2),
      m_message_level(m_text_level, font),
      m_message_game_won(m_text_won, font),
      m_message_game_lost(m_text_lost, font),
      m_message_lives(m_text_lives, font),
      m_message_game_name(m_game_name, font),
      m_button(m_window.getSize().x / 7, m_window.getSize().x / 17, m_play_text, font)
{
    load_animation();

    float size = m_window.getSize().x / 15;
    sf::Text::Style style = sf::Text::Style::Regular;
    sf::Color fill = sf::Color::White;
    sf::Color outline = sf::Color::White;

    m_message_game_won.set_properties(size, style, fill, outline, 2);
    m_message_game_lost.set_properties(size, style, fill, outline, 2);
    m_message_game_name.set_properties(size, style, fill, outline, 2);
    m_message_level.set_properties(size / 3, style, fill, outline, 2);
    m_message_lives.set_properties(size / 3, style, fill, outline, 2);

    load_levels();
    std::cout << "load_levels is done" << std::endl;

    load_blocks();
    std::cout << "load_blocks is done" << std::endl;
}

void Game::load_animation()
{
    for (size_t i = 0; i < m_window.getSize().x / 20; i++)
    {
        m_stars.push_back(std::make_unique<Star>(m_window));
    }
}

void Game::update_animation()
{
    for (auto &star : m_stars)
    {
        star->update(m_window);
    }

    m_message_game_name.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2 - 100);

    m_button.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2 + 100);

    if (m_button.is_pressed(m_window))
    {
        m_game_started = true;
    }
}

void Game::run_animation()
{
    for (auto &star : m_stars)
    {
        star->draw(m_window);
    }

    m_message_game_name.show_message(m_window);

    m_button.draw(m_window);
}

bool Game::load_levels()
{
    std::fstream fs("levels.data");

    if (!fs)
    {
        return false;
    }

    for (;;)
    {
        std::string line;

        if (!std::getline(fs, line))
        {
            break;
        }

        m_titles.push_back(line);

        if (!std::getline(fs, line))
        {
            std::cout << "height is not found" << std::endl;
            return false;
        }

        int height;
        std::istringstream ss(line);

        if (!(ss >> height))
        {
            std::cout << "height is not an int" << std::endl;
            return false;
        }

        std::vector<std::string> level;

        for (int i = 0; i < height; i++)
        {
            if (!std::getline(fs, line))
            {
                std::cout << "level is not found" << std::endl;
                return false;
            }

            level.push_back(line);
        }

        m_levels.push_back(level);
    }

    return true;
}

void Game::load_blocks()
{
    sf::Vector2f block_size(m_window.getSize().x / m_levels[m_curr_level][0].size(), m_window.getSize().y / m_levels[m_curr_level].size());

    std::unordered_map<char, std::pair<sf::Color, int>> block_types = {{'a', {sf::Color::Green, 1}}, {'b', {sf::Color::Magenta, 2}}, {'c', {sf::Color(235, 210, 52), NAN}}};

    for (size_t i = 0; i < m_levels[m_curr_level].size(); i++)
    {
        for (size_t j = 0; j < m_levels[m_curr_level][i].size(); j++)
        {
            char type = m_levels[m_curr_level][i][j];

            if (type != '.')
            {
                std::pair block_type = block_types[type];
                sf::Vector2f pos(j * block_size.x + 2, i * block_size.y + 2);

                m_blocks.push_back(std::make_unique<Block>(m_window, block_size, pos, block_type.first, block_type.second));
            }
        }
    }
}

void Game::restart()
{
    if (m_game_state == GameState::Lost)
    {
        for (auto &block : m_blocks)
        {
            block->heal();
        }
    }
    else if (m_game_state == GameState::Won)
    {
        if (++m_curr_level == m_levels.size())
        {
            m_curr_level = 0;
        }

        m_blocks.clear();
        load_blocks();
    }

    m_game_state = GameState::Default;
    m_start_moving = false;

    m_player.set_pos(m_window.getSize().x / 2);

    m_ball.set_pos(m_window.getSize().x / 2, m_player.get_pos().y - m_player.get_size().y * 2);
    m_ball.set_missed(false);
    m_ball.set_lost(false);
    m_ball.resume_lives();
}

void Game::run()
{
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                m_window.close();
            }
            else if (m_game_started && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                m_start_moving = true;
            }
            else if ((m_game_state == GameState::Won || m_game_state == GameState::Lost) &&
                     sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                restart();
            }

            if (m_game_started && m_start_moving)
            {
                m_player.set_pos(sf::Mouse::getPosition(m_window).x);
            }
        }

        m_total_time += m_clock.restart();

        while (m_total_time > m_frames_per_sec)
        {
            m_total_time -= m_frames_per_sec;

            if (m_game_started && m_start_moving && m_game_state != GameState::Won)
            {
                m_ball.move(m_frames_per_sec.asSeconds(), m_player, m_blocks);
            }
        }

        if (m_ball.is_lost())
        {
            m_game_state = GameState::Lost;
        }
        else if (m_ball.is_missed())
        {
            m_start_moving = false;

            m_player.set_pos(m_window.getSize().x / 2);

            m_ball.set_pos(m_window.getSize().x / 2, m_player.get_pos().y - m_player.get_size().y * 2);
            m_ball.set_missed(false);
            m_ball.set_lost(false);
        }
        else
        {
            size_t counter = 0;

            for (auto &block : m_blocks)
            {
                if (block->is_ruined() || block->get_initial_health() == NAN)
                {
                    ++counter;
                }
            }

            if (counter == m_blocks.size())
            {
                m_game_state = GameState::Won;
            }
        }

        if (m_game_state == GameState::Lost)
        {
            m_message_game_lost.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2);
        }
        else if (m_game_state == GameState::Won)
        {
            m_message_game_won.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2);
        }

        if (!m_game_started)
        {
            update_animation();
        }

        std::string level = m_text_level + m_titles[m_curr_level];
        m_message_level.set_str(level);
        m_message_level.set_pos(m_blocks.front()->get_size().x * 2, m_blocks.front()->get_size().y);

        std::string lives = m_text_lives + std::to_string(m_ball.get_remained_lives());
        m_message_lives.set_str(lives);
        m_message_lives.set_pos(m_window.getSize().x - m_blocks.front()->get_size().x * 2, m_blocks.front()->get_size().y);

        // draw
        if (!m_game_started)
        {
            sf::RectangleShape rect;
            rect.setPosition(0, 0);
            rect.setSize(sf::Vector2f(m_window.getSize().x, m_window.getSize().y));
            rect.setFillColor(sf::Color(0, 0, 0, 100));

            m_window.draw(rect);

            run_animation();
        }
        else
        {
            m_window.clear();

            m_player.draw();

            for (auto &block : m_blocks)
            {
                if (!block->is_ruined())
                {
                    block->draw();
                }
            }

            m_ball.draw();

            if (m_game_state == GameState::Won)
            {
                m_message_game_won.show_message(m_window);
            }
            else if (m_game_state == GameState::Lost)
            {
                m_message_game_lost.show_message(m_window);
            }

            m_message_level.show_message(m_window);
            m_message_lives.show_message(m_window);
        }

        m_window.display();
    }
}