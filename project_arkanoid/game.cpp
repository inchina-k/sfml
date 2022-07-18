#include "game.hpp"

#include "../libs/random.hpp"

using Random = effolkronium::random_static;

Game::Game(sf::RenderWindow &window, sf::Font &font)
    : m_window(window), m_frames_per_sec(sf::seconds(0.01f)), m_total_time(sf::Time::Zero),
      m_player(m_window, m_window.getSize().x / 2, m_window.getSize().y - m_player.get_size().y - 30),
      m_ball(m_window, m_window.getSize().x / 2, m_player.get_pos().y - m_player.get_size().y * 2),
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
    m_message_lives.set_properties(size / 3, style, fill, outline, 2);

    load_blocks();
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

void Game::load_blocks()
{
    const int num_of_blocks = 70;
    sf::Vector2f block_size(m_window.getSize().x / 15, m_window.getSize().y / 15);

    std::vector<std::pair<sf::Color, int>> block_types = {{sf::Color::Green, 1}, {sf::Color::Magenta, 2}};

    for (int i = 0; i < num_of_blocks; i++)
    {
        std::pair type = block_types[Random::get(0, int(block_types.size() - 1))];
        m_blocks.push_back(std::make_unique<Block>(m_window, block_size, type.first, type.second));
    }
}

void Game::place_blocks()
{
    sf::Vector2f block_size = m_blocks.front()->get_size();
    float x = m_window.getSize().x / 2 - block_size.x * 5 - 2;
    float y = m_window.getSize().y / 2 - block_size.y * 5 - 2;

    sf::Vector2f pos(x, y);
    int index = 0;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            m_blocks[index++]->set_pos(pos.x, pos.y);
            pos.x += block_size.x + 2;
        }

        pos.x = x;
        pos.y += block_size.y + 2;
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
                if (block->is_ruined())
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

        m_game_started ? place_blocks() : update_animation();

        std::string str = m_text_lives + std::to_string(m_ball.get_remained_lives());
        m_message_lives.set_str(str);
        m_message_lives.set_pos(m_window.getSize().x - m_blocks.front()->get_size().x * 3, m_blocks.front()->get_size().y);

        // draw
        m_window.clear();

        if (!m_game_started)
        {
            run_animation();
        }
        else
        {
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

            m_message_lives.show_message(m_window);
        }

        m_window.display();
    }
}