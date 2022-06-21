#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML app");
    window.setVerticalSyncEnabled(true);

    sf::CircleShape shape;
    shape.setRadius(100);

    float shapeX = window.getSize().x / 2 - shape.getRadius();
    float shapeY = window.getSize().y / 2 - shape.getRadius();

    shape.setPosition(sf::Vector2f(shapeX, shapeY));
    shape.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                shape.setPosition(shapeX, shapeY - shape.getRadius() * 2);
                shape.setFillColor(sf::Color::Red);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                shape.setPosition(shapeX + shape.getRadius() * 2, shapeY);
                shape.setFillColor(sf::Color::Magenta);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                shape.setPosition(shapeX - shape.getRadius() * 2, shapeY);
                shape.setFillColor(sf::Color::White);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                shape.setPosition(shapeX, shapeY + shape.getRadius() * 2);
                shape.setFillColor(sf::Color::Blue);
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}