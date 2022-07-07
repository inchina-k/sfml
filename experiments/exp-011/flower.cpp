#include "flower.hpp"
#include "../../libs/random.hpp"

using Random = effolkronium::random_static;

Flower::Flower(sf::RenderWindow &window)
    : m_angle(0), m_color(m_colors[Random::get(0, int(m_colors.size() - 1))])
{
    m_size = Random::get(M_MIN_SZ, M_MAX_SZ);

    m_x = Random::get(0, int(window.getSize().x));
    m_y = Random::get(0, int(window.getSize().y));
    m_dx = Random::get(M_MIN_SPEED, m_size / 20);
    m_dy = Random::get(M_MIN_SPEED, m_size / 20);
    m_da = Random::get(0.7, 1.0);

    if (Random::get(0, 1) == 0)
    {
        m_dx = -m_dx;
    }

    if (Random::get(0, 1) == 0)
    {
        m_dy = -m_dy;
    }

    line_1.setSize(sf::Vector2f(3, m_size));
    line_1.setOrigin(line_1.getSize().x / 2, line_1.getSize().y / 2);
    line_1.setFillColor(m_color);

    line_2.setSize(sf::Vector2f(m_size, 3));
    line_2.setOrigin(line_2.getSize().x / 2, line_2.getSize().y / 2);
    line_2.setFillColor(m_color);

    line_3.setSize(sf::Vector2f(3, m_size));
    line_3.setOrigin(line_3.getSize().x / 2, line_3.getSize().y / 2);
    line_3.setFillColor(m_color);

    line_4.setSize(sf::Vector2f(3, m_size));
    line_4.setOrigin(line_4.getSize().x / 2, line_4.getSize().y / 2);
    line_4.setFillColor(m_color);

    line_5.setSize(sf::Vector2f(m_size, 3));
    line_5.setOrigin(line_5.getSize().x / 2, line_5.getSize().y / 2);
    line_5.setFillColor(m_color);

    line_6.setSize(sf::Vector2f(m_size, 3));
    line_6.setOrigin(line_6.getSize().x / 2, line_6.getSize().y / 2);
    line_6.setFillColor(m_color);

    set_pos();

    line_3.rotate(30);
    line_4.rotate(-30);
    line_5.rotate(30);
    line_6.rotate(-30);
}

void Flower::setup(sf::RenderWindow &window)
{
    line_1.setRotation(m_angle);
    line_2.setRotation(m_angle);
    line_3.setRotation(m_angle);
    line_4.setRotation(m_angle);
    line_5.setRotation(m_angle);
    line_6.setRotation(m_angle);

    if (m_x + m_size / 2 >= window.getSize().x)
    {
        m_dx = -m_dx;
        m_x = window.getSize().x - m_size / 2 - 1;
        m_da = -m_da;
    }
    if (m_x - m_size / 2 < 0)
    {
        m_dx = -m_dx;
        m_x = m_size / 2;
        m_da = -m_da;
    }

    if (m_y + m_size / 2 >= window.getSize().y)
    {
        m_dy = -m_dy;
        m_y = window.getSize().y - m_size / 2 - 1;
        m_da = -m_da;
    }
    if (m_y - m_size / 2 < 0)
    {
        m_dy = -m_dy;
        m_y = m_size / 2;
        m_da = -m_da;
    }

    m_x += m_dx;
    m_y += m_dy;
    m_angle += m_da;

    set_pos();

    line_3.rotate(30);
    line_4.rotate(-30);
    line_5.rotate(30);
    line_6.rotate(-30);
}

void Flower::draw(sf::RenderWindow &window)
{
    window.draw(line_1);
    window.draw(line_2);
    window.draw(line_3);
    window.draw(line_4);
    window.draw(line_5);
    window.draw(line_6);
}

void Flower::set_pos()
{
    line_1.setPosition(m_x, m_y);
    line_2.setPosition(m_x, m_y);
    line_3.setPosition(m_x, m_y);
    line_4.setPosition(m_x, m_y);
    line_5.setPosition(m_x, m_y);
    line_6.setPosition(m_x, m_y);
}