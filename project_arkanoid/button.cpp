#include "button.hpp"

Button::Button(float sz_x, float sz_y, std::string &text, sf::Font &font)
    : m_body(sf::Vector2f(sz_x, sz_y)), m_text(text, font)
{
    m_body.setOrigin(m_body.getSize().x / 2, m_body.getSize().y / 2);
    m_body.setFillColor(sf::Color(112, 112, 112));
    m_body.setOutlineColor(sf::Color::White);
    m_body.setOutlineThickness(2);

    m_text.setFillColor(sf::Color::White);
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

bool Button::is_aimed(sf::RenderWindow &window)
{
    if (sf::Mouse::getPosition(window).x >= m_x - m_body.getSize().x / 2 &&
        sf::Mouse::getPosition(window).x <= m_x + m_body.getSize().x / 2 &&
        sf::Mouse::getPosition(window).y >= m_y - m_body.getSize().y / 2 &&
        sf::Mouse::getPosition(window).y <= m_y + m_body.getSize().y / 2)
    {
        m_body.setFillColor(sf::Color(153, 153, 153));
        return true;
    }

    m_body.setFillColor(sf::Color(112, 112, 112));
    return false;
}

bool Button::is_pressed(sf::RenderWindow &window)
{
    return is_aimed(window) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(m_body);
    window.draw(m_text);
}