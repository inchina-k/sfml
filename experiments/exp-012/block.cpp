#include "block.hpp"

Block::Block(sf::RenderWindow &window, float x, float y)
    : m_window(window), m_body(sf::Vector2f(m_window.getSize().x / 4, m_window.getSize().y / 4)), m_x(x), m_y(y)
{
    m_body.setFillColor(sf::Color::Green);
    m_body.setOrigin(m_body.getSize().x / 2, m_body.getSize().y / 2);
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
    return x >= m_body.getPosition().x - m_body.getSize().x / 2 &&
           x <= m_body.getPosition().x + m_body.getSize().x / 2 &&
           y >= m_body.getPosition().y + m_body.getSize().y / 2;
}

bool Block::is_above(float x, float y) const
{
    return x >= m_body.getPosition().x - m_body.getSize().x / 2 &&
           x <= m_body.getPosition().x + m_body.getSize().x / 2 &&
           y <= m_body.getPosition().y - m_body.getSize().y / 2;
}

bool Block::is_left(float x, float y) const
{
    return y >= m_body.getPosition().y - m_body.getSize().y / 2 &&
           y <= m_body.getPosition().y + m_body.getSize().y / 2 &&
           x <= m_body.getPosition().x - m_body.getSize().x / 2;
}

bool Block::is_right(float x, float y) const
{
    return y >= m_body.getPosition().y - m_body.getSize().y / 2 &&
           y <= m_body.getPosition().y + m_body.getSize().y / 2 &&
           x >= m_body.getPosition().x + m_body.getSize().x / 2;
}

bool Block::in_rect(float tx, float ty, float radius) const
{
    return tx >= m_body.getPosition().x - m_body.getSize().x / 2 - radius &&
           tx <= m_body.getPosition().x + m_body.getSize().x / 2 + radius &&
           ty <= m_body.getPosition().y + m_body.getSize().y / 2 + radius &&
           ty >= m_body.getPosition().y - m_body.getSize().y / 2 - radius;
}

void Block::draw()
{
    m_window.draw(m_body);
}