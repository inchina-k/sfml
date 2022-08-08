#include "player.hpp"

Player::Player(sf::RenderWindow &window, float x, float y)
    : m_window(window), m_size(m_window.getSize().x / 10, m_window.getSize().x / 100), m_x(x), m_y(y)
{
    m_texture.loadFromFile("data/images/player.png");
    m_body.setTexture(&m_texture);
    m_body.setPosition(m_x, m_y);
    m_body.setSize(m_size);
    m_body.setOrigin(m_body.getSize().x / 2, m_body.getSize().y / 2);
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

bool Player::is_above(float x, float y) const
{
    return x >= m_body.getPosition().x - m_body.getSize().x / 2 &&
           x <= m_body.getPosition().x + m_body.getSize().x / 2 &&
           y <= m_body.getPosition().y - m_body.getSize().y / 2;
}

bool Player::in_rect(float tx, float ty, float radius) const
{
    return tx >= m_body.getPosition().x - m_body.getSize().x / 2 - radius &&
           tx <= m_body.getPosition().x + m_body.getSize().x / 2 + radius &&
           ty <= m_body.getPosition().y + m_body.getSize().y / 2 + radius &&
           ty >= m_body.getPosition().y - m_body.getSize().y / 2 - radius;
}

void Player::increase_size()
{
    m_body.setSize(sf::Vector2f(m_size.x * 1.5, m_size.y));
    m_body.setOrigin(m_body.getSize().x / 2, m_body.getSize().y / 2);
}

void Player::restore_size()
{
    m_body.setSize(m_size);
    m_body.setOrigin(m_body.getSize().x / 2, m_body.getSize().y / 2);
}

void Player::draw()
{
    m_window.draw(m_body);
}