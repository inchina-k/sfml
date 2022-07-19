#include "star.hpp"

#include "../libs/random.hpp"

using Random = effolkronium::random_static;

Star::Star(sf::RenderWindow &window)
    : m_radius(Random::get(5, 10)),
      m_pos(Random::get(0.0f, float(window.getSize().x)), Random::get(0.0f, float(window.getSize().y)))
{
    m_body.setRadius(m_radius);
    m_body.setOrigin(m_body.getRadius(), m_body.getRadius());
    m_body.setFillColor(m_colors[Random::get(0, int(m_colors.size() - 1))]);

    load(window);
}

void Star::load(sf::RenderWindow &window)
{
    float dx = m_pos.x - window.getSize().x / 2;
    float dy = m_pos.y - window.getSize().y / 2;

    float r = std::sqrt(dx * dx + dy * dy);

    float cos = dx / r;
    float sin = dy / r;

    m_speed.x = std::max(4.0f, r / 150) * cos;
    m_speed.y = std::max(4.0f, r / 150) * sin;
}

void Star::update(sf::RenderWindow &window)
{
    m_body.setPosition(m_pos);
    m_body.setRadius(m_body.getRadius() + 0.01);

    if (m_pos.x > window.getSize().x + m_body.getRadius() || m_pos.x < -m_body.getRadius() ||
        m_pos.y > window.getSize().y + m_body.getRadius() || m_pos.y < -m_body.getRadius())
    {
        m_pos.x = Random::get(0.0f, float(window.getSize().x));
        m_pos.y = Random::get(0.0f, float(window.getSize().y));

        load(window);

        m_body.setRadius(m_radius);
    }

    m_pos += m_speed;
}

void Star::draw(sf::RenderWindow &window)
{
    window.draw(m_body);
}