#include "SFML/Graphics.hpp"

#include <string>

class Button
{
    sf::RenderWindow &m_window;
    sf::RectangleShape m_body;
    float m_x;
    float m_y;
    sf::Text m_text;

public:
    Button(sf::RenderWindow &window, float sz_x, float sz_y, std::string &text, sf::Font &font);

    void set_pos(float x, float y);

    bool is_aimed();

    bool is_pressed();

    void draw();
};