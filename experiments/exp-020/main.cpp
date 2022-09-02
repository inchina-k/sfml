#include <SFML/Graphics.hpp>

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    float shapeX = window.getSize().x / 2;
    float shapeY = window.getSize().y / 2;

    sf::View view;
    view.setCenter(window.getSize().x, window.getSize().y);
    view.setSize(window.getSize().x, window.getSize().y);

    sf::View map_view;
    map_view.setCenter(window.getSize().x, window.getSize().y);
    map_view.setSize(window.getSize().x * 2, window.getSize().y * 2);
    map_view.setViewport(sf::FloatRect(0.75f, 0.75f, 0.25f, 0.25f));

    sf::View text_view;
    text_view.setCenter(window.getSize().x / 2, window.getSize().y / 2);
    text_view.setSize(window.getSize().x, window.getSize().y);

    sf::CircleShape shape_1;
    sf::CircleShape shape_2;
    sf::CircleShape shape_3;
    sf::CircleShape shape_4;
    sf::CircleShape shape_5;

    sf::RectangleShape rect;

    shape_1.setRadius(70);
    shape_2.setRadius(70);
    shape_3.setRadius(70);
    shape_4.setRadius(70);
    shape_5.setRadius(100);

    rect.setSize(sf::Vector2f(60, 60));

    shape_1.setOrigin(shape_1.getRadius(), shape_1.getRadius());
    shape_2.setOrigin(shape_2.getRadius(), shape_2.getRadius());
    shape_3.setOrigin(shape_3.getRadius(), shape_3.getRadius());
    shape_4.setOrigin(shape_4.getRadius(), shape_4.getRadius());
    shape_5.setOrigin(shape_5.getRadius(), shape_5.getRadius());

    rect.setOrigin(30, 30);

    sf::Vector2f s1(shapeX, shapeY);
    sf::Vector2f s2(shapeX, shapeY + window.getSize().y);
    sf::Vector2f s3(shapeX + window.getSize().x, shapeY);
    sf::Vector2f s4(shapeX + window.getSize().x, shapeY + window.getSize().y);
    sf::Vector2f s5(shapeX + shapeX, shapeY + shapeY);

    shape_1.setPosition(s1);
    shape_2.setPosition(s2);
    shape_3.setPosition(s3);
    shape_4.setPosition(s4);
    shape_5.setPosition(s5);

    rect.setPosition(s5);

    shape_1.setFillColor(sf::Color::Red);
    shape_2.setFillColor(sf::Color::Blue);
    shape_3.setFillColor(sf::Color::Green);
    shape_4.setFillColor(sf::Color::Yellow);
    shape_5.setFillColor(sf::Color::White);

    rect.setFillColor(sf::Color::Cyan);

    sf::Font font;
    font.loadFromFile("PressStart2P-Regular.ttf");

    sf::Text title("Bomberman", font);
    title.setFillColor(sf::Color::Magenta);
    title.setCharacterSize(30);
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(text_view.getCenter().x, 100);

    sf::Text pos_x("x: " + std::to_string(int(rect.getPosition().x)), font);
    pos_x.setFillColor(sf::Color::Magenta);
    pos_x.setCharacterSize(30);
    pos_x.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);

    sf::Text pos_y("y: " + std::to_string(int(rect.getPosition().y)), font);
    pos_y.setFillColor(sf::Color::Magenta);
    pos_y.setCharacterSize(30);
    pos_y.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (view.getCenter().x > s1.x && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                view.move(-10.0f, 0);
            }
            if (view.getCenter().x < s4.x && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                view.move(10.0f, 0);
            }
            if (view.getCenter().y > s1.y && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                view.move(0, -10.0f);
            }
            if (view.getCenter().y < s4.y && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                view.move(0, 10.0f);
            }

            rect.setPosition(view.getCenter());

            pos_x.setString("x: " + std::to_string(int(rect.getPosition().x)));
            pos_y.setString("y: " + std::to_string(int(rect.getPosition().y)));
            pos_x.setPosition(text_view.getCenter().x - 300, text_view.getCenter().y + text_view.getSize().y / 2 - 30);
            pos_y.setPosition(text_view.getCenter().x + 300, text_view.getCenter().y + text_view.getSize().y / 2 - 30);
        }

        window.clear();

        window.setView(view);
        window.draw(shape_1);
        window.draw(shape_2);
        window.draw(shape_3);
        window.draw(shape_4);
        window.draw(shape_5);
        window.draw(rect);

        window.setView(map_view);
        window.draw(shape_1);
        window.draw(shape_2);
        window.draw(shape_3);
        window.draw(shape_4);
        window.draw(shape_5);
        window.draw(rect);

        window.setView(text_view);
        window.draw(title);
        window.draw(pos_x);
        window.draw(pos_y);

        window.display();
    }
}