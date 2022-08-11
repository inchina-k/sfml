#include "cell.hpp"

Cell::Cell(sf::RenderWindow &window, sf::Vector2f &size, sf::Vector2f &pos, sf::Texture &texture)
    : m_window(window), m_body(size), m_pos(pos), m_texture(texture)
{
    m_body.setTexture(&m_texture);
    m_body.setPosition(m_pos);
}

sf::Vector2f Cell::get_pos() const
{
    return m_body.getPosition();
}

sf::Vector2f Cell::get_size() const
{
    return m_body.getSize();
}

bool Cell::in_rect(float tx, float ty, float radius) const
{
    return tx >= m_body.getPosition().x - radius &&
           tx <= m_body.getPosition().x + m_body.getSize().x + radius &&
           ty <= m_body.getPosition().y + m_body.getSize().y + radius &&
           ty >= m_body.getPosition().y - radius;
}

void Cell::draw()
{
    m_window.draw(m_body);
}