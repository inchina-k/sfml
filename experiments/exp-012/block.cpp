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

void Block::draw()
{
    m_window.draw(m_body);
}