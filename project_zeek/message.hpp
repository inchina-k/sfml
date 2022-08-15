#pragma once

#include <SFML/Graphics.hpp>

#include <string>

class Message
{
    sf::Text m_text;

public:
    Message(std::string &message, sf::Font &font);

    void set_properties(int size, sf::Text::Style &style, sf::Color &fill, sf::Color &outline, int thickness);

    void set_pos(int position_x, int position_y);

    void set_str(std::string &message);

    void show_message(sf::RenderWindow &window);
};