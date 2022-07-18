#include "block.hpp"

Block::Block(sf::RenderWindow &window, sf::Vector2f &size, sf::Color &color, int health)
    : m_window(window), m_body(size), m_initial_color(color), m_initial_health(health), m_health(health)
{
    m_body.setFillColor(m_initial_color);
    m_body.setOutlineColor(sf::Color::White);
    m_body.setOutlineThickness(2);
}

void Block::set_pos(float x, float y)
{
    m_x = x;
    m_y = y;

    m_body.setPosition(m_x, m_y);
}

sf::Vector2f Block::get_pos() const
{
    return m_body.getPosition();
}

sf::Vector2f Block::get_size() const
{
    return m_body.getSize();
}

bool Block::is_below(float x, float y) const
{
    return x >= m_body.getPosition().x &&
           x <= m_body.getPosition().x + m_body.getSize().x &&
           y >= m_body.getPosition().y + m_body.getSize().y;
}

bool Block::is_above(float x, float y) const
{
    return x >= m_body.getPosition().x &&
           x <= m_body.getPosition().x + m_body.getSize().x &&
           y <= m_body.getPosition().y;
}

bool Block::is_left(float x, float y) const
{
    return y >= m_body.getPosition().y &&
           y <= m_body.getPosition().y + m_body.getSize().y &&
           x <= m_body.getPosition().x;
}

bool Block::is_right(float x, float y) const
{
    return y >= m_body.getPosition().y &&
           y <= m_body.getPosition().y + m_body.getSize().y &&
           x >= m_body.getPosition().x + m_body.getSize().x;
}

bool Block::in_rect(float tx, float ty, float radius) const
{
    return tx >= m_body.getPosition().x - radius &&
           tx <= m_body.getPosition().x + m_body.getSize().x + radius &&
           ty <= m_body.getPosition().y + m_body.getSize().y + radius &&
           ty >= m_body.getPosition().y - radius;
}

bool Block::is_ruined() const
{
    return !m_health;
}

void Block::heal()
{
    m_health = m_initial_health;
    m_body.setFillColor(m_initial_color);
}

void Block::reduce_health()
{
    if (--m_health > 0)
    {
        m_body.setFillColor(sf::Color(102, 16, 93));
    }
}

void Block::draw()
{
    m_window.draw(m_body);
}