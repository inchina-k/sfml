#include "star.hpp"

#include "../libs/random.hpp"

using Random = effolkronium::random_static;

Star::Star(sf::RenderWindow &window)
    : m_radius(Random::get(10, 15)),
      m_x(Random::get(0.0f, float(window.getSize().x))),
      m_y(Random::get(0.0f, float(window.getSize().y))),
      m_z(Random::get(3.0f, 5.0f))
{
    m_body.setRadius(m_radius);
    m_body.setOrigin(m_body.getRadius(), m_body.getRadius());
    m_body.setFillColor(m_colors[Random::get(0, int(m_colors.size() - 1))]);
}

void Star::update(sf::RenderWindow &window)
{
    m_body.setPosition(m_x, m_y);
    m_body.setRadius(m_body.getRadius() + 0.1);

    m_x >= window.getSize().x / 2 ? m_x += m_z : m_x -= m_z;
    m_y >= window.getSize().y / 2 ? m_y += m_z : m_y -= m_z;

    if (m_x > window.getSize().x + m_body.getRadius() || m_x < -m_body.getRadius() ||
        m_y > window.getSize().y + m_body.getRadius() || m_y < -m_body.getRadius())
    {
        m_x = Random::get(0.0f, float(window.getSize().x));
        m_y = Random::get(0.0f, float(window.getSize().y));

        m_body.setRadius(m_radius);
    }
}

void Star::draw(sf::RenderWindow &window)
{
    window.draw(m_body);
}