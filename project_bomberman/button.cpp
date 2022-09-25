#include "button.hpp"

Button::Button(sf::RenderWindow &window, float sz_x, float sz_y, std::string &text, sf::Font &font)
    : m_window(window), m_body(sf::Vector2f(sz_x, sz_y)), m_text(text, font)
{
    m_body.setOrigin(m_body.getSize().x / 2, m_body.getSize().y / 2);
    m_body.setFillColor(sf::Color::White);
    m_body.setOutlineColor(sf::Color(101, 140, 164));
    m_body.setOutlineThickness(2);

    m_text.setFillColor(sf::Color(148, 204, 236));
    m_text.setCharacterSize(m_body.getSize().y / 2.5);
}

void Button::set_pos(float x, float y)
{
    m_x = x;
    m_y = y;

    m_body.setPosition(m_x, m_y);

    float origin_x = m_text.getLocalBounds().width / 2;
    float origin_y = m_text.getLocalBounds().height / 2;

    m_text.setOrigin(origin_x, origin_y);
    m_text.setPosition(m_x, m_y);
}

bool Button::is_aimed()
{
    if (sf::Mouse::getPosition(m_window).x >= m_x - m_body.getSize().x / 2 &&
        sf::Mouse::getPosition(m_window).x <= m_x + m_body.getSize().x / 2 &&
        sf::Mouse::getPosition(m_window).y >= m_y - m_body.getSize().y / 2 &&
        sf::Mouse::getPosition(m_window).y <= m_y + m_body.getSize().y / 2)
    {
        m_body.setFillColor(sf::Color(148, 204, 236));
        m_body.setOutlineColor(sf::Color::White);
        m_text.setFillColor(sf::Color::White);

        return true;
    }

    m_body.setFillColor(sf::Color::White);
    m_body.setOutlineColor(sf::Color(101, 140, 164));
    m_text.setFillColor(sf::Color(148, 204, 236));

    return false;
}

bool Button::is_pressed()
{
    return is_aimed() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

void Button::draw()
{
    m_window.draw(m_body);
    m_window.draw(m_text);
}