#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML app");
    window.setVerticalSyncEnabled(true);

    sf::Font font;
    font.loadFromFile("data/Parisienne-Regular.ttf");

    sf::Text text("Good morning, starshine...\nThe Earth says \"Hello!\"", font);
    text.setCharacterSize(50);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Magenta);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            float sz_w = text.getLocalBounds().width;
            float sz_h = text.getLocalBounds().height;

            text.setOrigin(sz_w / 2, sz_h / 2);
            text.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        }

        window.clear();
        window.draw(text);
        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}