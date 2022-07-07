#pragma once

#include "SFML/Graphics.hpp"

class BackgroundAnimation
{
protected:
    sf::RenderWindow &m_window;
    int m_num_of_objects;

public:
    BackgroundAnimation(sf::RenderWindow &window, int num_of_objects)
        : m_window(window), m_num_of_objects(num_of_objects)
    {
    }

    virtual void load_animation() = 0;

    virtual void draw_animation() = 0;
};