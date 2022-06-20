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

    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("data/Explosion.wav"))
    {
        std::cout << "data/Explosion.wav not found" << std::endl;
        return 1;
    }

    sf::Sound sound;
    sound.setBuffer(soundBuffer);
    sound.setVolume(100);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                sound.play();
#ifdef AUCA_DEBUG
                std::clog << sound.getVolume() << std::endl;
#endif
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