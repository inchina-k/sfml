#include <SFML/Graphics.hpp>

#include <iostream>
#include "background_animation.hpp"
#include "snowflakes.hpp"
#include "flowers.hpp"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML app", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    size_t bg_anim_index = 0;

    vector<unique_ptr<BackgroundAnimation>> bg_animations;

    bg_animations.push_back(make_unique<Snowflakes>(window, window.getSize().x / 20));
    bg_animations.push_back(make_unique<Flowers>(window, window.getSize().x / 20));

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
            {
                ++bg_anim_index;

                if (bg_anim_index == bg_animations.size())
                {
                    bg_anim_index = 0;
                }
            }
        }

        bg_animations[bg_anim_index]->load_animation();

        window.clear();
        bg_animations[bg_anim_index]->draw_animation();
        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}