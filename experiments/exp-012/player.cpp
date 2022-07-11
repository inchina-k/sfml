#include "player.hpp"

Player::Player(sf::RenderWindow &window, float x, float y)
    : m_window(window), m_body(sf::Vector2f(m_window.getSize().x / 10, 5)), m_x(x), m_y(y)
{
    m_body.setFillColor(sf::Color::Cyan);
    m_body.setOrigin(m_body.getSize().x / 2, m_body.getSize().y / 2);
    m_body.setPosition(m_x, m_y);
}

void Player::set_pos(float x)
{
    if (x >= m_body.getSize().x / 2 && x <= m_window.getSize().x - m_body.getSize().x / 2)
    {
        m_x = x;
    }

    m_body.setPosition(m_x, m_y);
}

sf::Vector2f Player::get_pos() const
{
    return m_body.getPosition();
}

sf::Vector2f Player::get_size() const
{
    return m_body.getSize();
}

void Player::draw()
{
    m_window.draw(m_body);
}