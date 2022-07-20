#include "block.hpp"

Block::Block(sf::RenderWindow &window, sf::Vector2f &size, sf::Vector2f &pos, sf::Texture &texture, int health)
    : m_window(window), m_body(size), m_pos(pos), m_initial_texture(texture), m_initial_health(health), m_health(health)
{
    m_body.setTexture(&m_initial_texture);
    m_body.setPosition(m_pos);
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
    m_body.setFillColor(sf::Color(255, 255, 255));
}

void Block::reduce_health()
{
    if (--m_health > 0 && m_health < m_initial_health)
    {
        m_body.setFillColor(sf::Color(181, 147, 180));
    }
}

int Block::get_initial_health() const
{
    return m_initial_health;
}

void Block::draw()
{
    m_window.draw(m_body);
}