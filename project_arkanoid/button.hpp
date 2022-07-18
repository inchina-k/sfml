#include "SFML/Graphics.hpp"

#include <string>

class Button
{
    sf::RectangleShape m_body;
    float m_x;
    float m_y;
    sf::Text m_text;

public:
    Button(float sz_x, float sz_y, std::string &text, sf::Font &font);

    void set_pos(float x, float y);

    bool is_aimed(sf::RenderWindow &window);

    bool is_pressed(sf::RenderWindow &window);

    void draw(sf::RenderWindow &window);
};