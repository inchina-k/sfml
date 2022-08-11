#include "player.hpp"

#include <iostream>

Player::Player(sf::RenderWindow &window)
    : m_window(window), m_counter(0), m_frame_index(0), m_anim_index(0), m_pos(0, 0), m_size(0)
{
    if (!m_texture.loadFromFile("data/images/doodly.png"))
    {
        std::cout << "file is missing" << std::endl;
        exit(1);
    }

    m_w = m_texture.getSize().x / 3;
    m_h = m_texture.getSize().y / 4;

    load();
}

void Player::set_state(State state)
{
    m_curr_state = state;

    if (m_curr_state == State::GoDown || m_curr_state == State::GoUp || m_curr_state == State::GoLeft || m_curr_state == State::GoRight)
    {
        m_anim_index = static_cast<int>(m_curr_state);
    }
}

void Player::load()
{
    m_frames.resize(4);

    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            m_frames[i].push_back(std::make_unique<sf::Sprite>(m_texture, sf::IntRect(j * m_w, i * m_h, m_w, m_h)));
        }
    }
}

void Player::set_pos(sf::Vector2f &coords)
{
    m_pos = coords;
}

void Player::set_size(float size)
{
    m_size = size;

    for (auto &frames : m_frames)
    {
        for (auto &frame : frames)
        {
            frame->scale(m_size / frame->getLocalBounds().width, m_size / frame->getLocalBounds().height);
            frame->setOrigin(frame->getLocalBounds().width / 2, frame->getLocalBounds().height / 2);
        }
    }
}

sf::Vector2f Player::get_pos() const
{
    return m_pos;
}

sf::Vector2f Player::get_size() const
{
    sf::Vector2f size(m_frames[m_anim_index][m_frame_index]->getLocalBounds().width, m_frames[m_anim_index][m_frame_index]->getGlobalBounds().height);
    return size;
}

void Player::move(sf::Vector2f &top, sf::Vector2f &bottom, float sz)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        set_state(Player::State::GoDown);
        if (m_pos.y + m_size / 2 + 2 <= bottom.y + sz)
        {
            m_pos.y += 2;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        set_state(Player::State::GoUp);
        if (m_pos.y - m_size / 2 - 2 >= top.y)
        {
            m_pos.y -= 2;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        set_state(Player::State::GoLeft);
        if (m_pos.x - m_size / 2 - 2 >= top.x)
        {
            m_pos.x -= 2;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        set_state(Player::State::GoRight);
        if (m_pos.x + m_size / 2 + 2 <= bottom.x + sz)
        {
            m_pos.x += 2;
        }
    }
    else if (m_curr_state == Player::State::GoDown)
    {
        set_state(Player::State::StandDown);
    }
    else if (m_curr_state == Player::State::GoUp)
    {
        set_state(Player::State::StandUp);
    }
    else if (m_curr_state == Player::State::GoLeft)
    {
        set_state(Player::State::StandLeft);
    }
    else if (m_curr_state == Player::State::GoRight)
    {
        set_state(Player::State::StandRight);
    }
}

void Player::draw()
{
    if (m_curr_state == State::StandDown || m_curr_state == State::StandUp || m_curr_state == State::StandLeft || m_curr_state == State::StandRight)
    {
        m_frame_index = 0;
    }

    m_frames[m_anim_index][m_frame_index]->setPosition(m_pos);
    m_window.draw(*m_frames[m_anim_index][m_frame_index]);

    if (++m_counter == 15)
    {
        if (++m_frame_index == 3)
        {
            m_frame_index = 1;
        }

        m_counter = 0;
    }
}