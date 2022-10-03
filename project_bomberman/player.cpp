#include "game.hpp"

#include <iostream>

Game::Player::Player(Game &game)
    : m_game(game), m_num_of_steps(m_max_counter / 1.7f), m_lives(3), m_lives_left(3)
{
    if (!m_texture.loadFromFile("data/images/player.png"))
    {
        std::cout << "file for the player is missing" << std::endl;
        exit(1);
    }

    m_w = m_texture.getSize().x / 6;
    m_h = m_texture.getSize().y / 4;

    load();
}

void Game::Player::load()
{
    m_frames.resize(4);

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            m_frames[i].push_back(std::make_unique<sf::Sprite>(m_texture, sf::IntRect(j * m_w, i * m_h, m_w, m_h)));
        }
    }
}

void Game::Player::set_size(sf::Vector2f &size)
{
    for (auto &frames : m_frames)
    {
        for (auto &frame : frames)
        {
            frame->setScale(size.x / frame->getLocalBounds().width, size.y / frame->getLocalBounds().height);
        }
    }
}

sf::Vector2f Game::Player::get_size() const
{
    sf::Vector2f size(m_frames[m_anim_index][m_frame_index]->getGlobalBounds().width,
                      m_frames[m_anim_index][m_frame_index]->getGlobalBounds().height);
    return size;
}

void Game::Player::set_pos(sf::Vector2f &pos)
{
    m_pos = pos;
}

sf::Vector2f Game::Player::get_pos() const
{
    return m_pos;
}

size_t Game::Player::get_lives() const
{
    return m_lives_left;
}

bool Game::Player::can_move()
{
    for (const auto &cells : m_game.m_cells)
    {
        for (const auto &cell : cells)
        {
            sf::FloatRect tmp(m_pos + m_dir, get_size());

            if (auto obj = dynamic_cast<Wall *>(cell.get()) &&
                           tmp.intersects(cell->get_bounds()))
            {
                return false;
            }
        }
    }

    return true;
}

void Game::Player::switch_command()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        m_anim_index = 0;

        m_dir.x = 0;
        m_dir.y = m_step;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        m_anim_index = 1;

        m_dir.x = 0;
        m_dir.y = -m_step;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        m_anim_index = 2;

        m_dir.x = -m_step;
        m_dir.y = 0;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        m_anim_index = 3;

        m_dir.x = m_step;
        m_dir.y = 0;
    }
    else
    {
        m_dir.x = 0;
        m_dir.y = 0;
    }
}

void Game::Player::move()
{
    if (can_move())
    {
        m_pos += m_dir;
        m_game.m_main_view.move(m_dir);
    }

    m_step = m_game.m_cells.front().back()->get_size().x / m_num_of_steps;

    switch_command();
}

void Game::Player::draw()
{
    m_frames[m_anim_index][m_frame_index]->setPosition(m_pos);
    m_game.m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    if (++m_counter == m_max_counter)
    {
        if (++m_frame_index == 6)
        {
            m_frame_index = 0;
        }

        m_counter = 0;
    }
}