#include "message.hpp"

Message::Message(std::string &message, sf::Font &font)
    : m_text(message, font)
{
}

void Message::set_properties(int size, sf::Text::Style &style, sf::Color &fill, sf::Color &outline, int thickness)
{
    m_text.setCharacterSize(size);
    m_text.setStyle(style);
    m_text.setFillColor(fill);
    m_text.setOutlineColor(outline);
    m_text.setOutlineThickness(thickness);
}

void Message::set_pos(int position_x, int position_y)
{
    double origin_x = m_text.getLocalBounds().width / 2;
    double origin_y = m_text.getLocalBounds().height / 2;

    m_text.setOrigin(origin_x, origin_y);
    m_text.setPosition(position_x, position_y);
}

void Message::set_str(std::string &message)
{
    m_text.setString(message);
}

void Message::show_message(sf::RenderWindow &window)
{
    window.draw(m_text);
}