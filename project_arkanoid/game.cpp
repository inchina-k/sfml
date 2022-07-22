#include "game.hpp"

#include "../libs/random.hpp"

#include <sstream>
#include <iostream>
#include <unordered_map>
#include <cmath>

using Random = effolkronium::random_static;

Game::Game(sf::RenderWindow &window, sf::Font &font,
           sf::Music &music, sf::Sound &sound_won, sf::Sound &sound_lost,
           sf::Sound &sound_hit, sf::Sound &sound_pop, sf::Sound &sound_unbreakable)
    : m_window(window), m_frames_per_sec(sf::seconds(0.01f)), m_total_time(sf::Time::Zero), m_curr_level(0),
      m_player(m_window, m_window.getSize().x / 2, m_window.getSize().y - m_player.get_size().y - 30),
      m_ball(m_window, m_window.getSize().x / 2, m_player.get_pos().y - m_player.get_size().y * 2, sound_hit, sound_pop, sound_unbreakable),
      m_music(music),
      m_sound_game_won(sound_won),
      m_sound_game_lost(sound_lost),
      m_message_level(m_text_level, font),
      m_message_game_state(m_text_won, font),
      m_message_lives(m_text_lives, font),
      m_message_game_name(m_game_name, font),
      m_button(m_window.getSize().x / 7, m_window.getSize().x / 17, m_play_text, font)
{
    m_music.setLoop(true);
    m_sound_game_won.setLoop(false);
    m_sound_game_lost.setLoop(false);

    load_animation();

    float size = m_window.getSize().x / 15;
    sf::Text::Style style = sf::Text::Style::Regular;
    sf::Color fill = sf::Color::White;
    sf::Color outline = sf::Color::White;

    m_message_game_state.set_properties(size, style, fill, outline, 2);
    m_message_game_name.set_properties(size, style, fill, outline, 2);
    m_message_level.set_properties(size / 3, style, fill, outline, 2);
    m_message_lives.set_properties(size / 3, style, fill, outline, 2);

    if (!load_levels())
    {
        exit(1);
    }
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
    std::fstream fs("data/levels.data");

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

    sf::Texture a_block, b_block, c_block;

    if (!a_block.loadFromFile("data/images/a_block.png") || !b_block.loadFromFile("data/images/b_block.png") || !c_block.loadFromFile("data/images/c_block.png"))
    {
        exit(1);
    }

    std::unordered_map<char, std::pair<sf::Texture, int>> block_types = {{'a', {a_block, 1}}, {'b', {b_block, 2}}, {'c', {c_block, -1}}};

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

    m_music.play();
}

void Game::run()
{
    m_music.play();

    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                m_window.close();
            }
            else if (m_game_started && m_game_state == GameState::Default && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
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

        if (m_ball.is_lost() && m_game_state == GameState::Default)
        {
            m_game_state = GameState::Lost;
            m_play = true;
        }
        else if (m_ball.is_missed())
        {
            m_start_moving = false;

            m_player.set_pos(m_window.getSize().x / 2);

            m_ball.set_pos(m_window.getSize().x / 2, m_player.get_pos().y - m_player.get_size().y * 2);
            m_ball.set_missed(false);
        }
        else
        {
            size_t counter = 0;

            for (auto &block : m_blocks)
            {
                if (block->is_ruined() || block->get_initial_health() == -1)
                {
                    ++counter;
                }
            }

            if (counter == m_blocks.size() && m_game_state == GameState::Default)
            {
                m_game_state = GameState::Won;
                m_play = true;
            }
        }

        if (m_game_state == GameState::Lost && m_play)
        {
            m_message_game_state.set_str(m_text_lost);

            m_music.stop();
            m_sound_game_lost.play();

            m_play = false;
        }
        else if (m_game_state == GameState::Won && m_play)
        {
            m_message_game_state.set_str(m_text_won);

            m_music.stop();
            m_sound_game_won.play();

            m_play = false;
        }
        m_message_game_state.set_pos(m_window.getSize().x / 2, m_window.getSize().y / 2);

        if (!m_game_started)
        {
            update_animation();
        }
        else
        {
            m_music.setVolume(15);
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

            if (m_game_state != GameState::Default)
            {
                m_message_game_state.show_message(m_window);
            }

            m_message_level.show_message(m_window);
            m_message_lives.show_message(m_window);
        }

        m_window.display();
    }
}