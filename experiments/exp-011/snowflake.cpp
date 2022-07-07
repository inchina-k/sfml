#include "snowflake.hpp"
#include "../../libs/random.hpp"

using Random = effolkronium::random_static;

Snowflake::Snowflake(sf::RenderWindow &window)
    : m_angle(0), m_color(sf::Color::White)
{
    m_size = Random::get(M_MIN_SZ, M_MAX_SZ);

    if (m_size < 50)
    {
        sf::Color new_color(170, 170, 170);
        m_color = new_color;
    }

    m_x = Random::get(0, int(window.getSize().x));
    m_y = Random::get(-50, int(window.getSize().y));
    m_dy = Random::get(M_MIN_SPEED, m_size / 20);
    m_da = Random::get(0.7, 1.0);
    m_rnd = m_da > 0.8 ? -m_da : m_da;

    line_1.setSize(sf::Vector2f(3, m_size));
    line_1.setOrigin(line_1.getSize().x / 2, line_1.getSize().y / 2);
    line_1.setFillColor(m_color);

    line_3.setSize(sf::Vector2f(m_size, 3));
    line_3.setOrigin(line_3.getSize().x / 2, line_3.getSize().y / 2);
    line_3.setFillColor(m_color);

    line_2.setSize(sf::Vector2f(m_size / 1.5, 3));
    line_2.setOrigin(line_2.getSize().x / 2, line_2.getSize().y / 2);
    line_2.setFillColor(m_color);

    line_4.setSize(sf::Vector2f(m_size / 1.5, 3));
    line_4.setOrigin(line_4.getSize().x / 2, line_4.getSize().y / 2);
    line_4.setFillColor(m_color);

    set_pos();

    line_2.rotate(45);
    line_4.rotate(-45);
}

void Snowflake::setup(sf::RenderWindow &window)
{
    if (m_y - m_size > window.getSize().y)
    {
        m_y = Random::get(-100, -200);
        m_x = Random::get(0, int(window.getSize().x));
    }

    line_1.setRotation(m_angle);
    line_2.setRotation(m_angle);
    line_3.setRotation(m_angle);
    line_4.setRotation(m_angle);

    m_y += m_dy;
    m_angle += m_rnd;

    set_pos();

    line_2.rotate(45);
    line_4.rotate(-45);
}

void Snowflake::draw(sf::RenderWindow &window)
{
    window.draw(line_1);
    window.draw(line_2);
    window.draw(line_3);
    window.draw(line_4);
}

void Snowflake::set_pos()
{
    line_1.setPosition(m_x, m_y);
    line_3.setPosition(m_x, m_y);
    line_2.setPosition(m_x, m_y);
    line_4.setPosition(m_x, m_y);
}