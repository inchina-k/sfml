#include "flowers.hpp"

Flowers::Flowers(sf::RenderWindow &window, int num_of_flowers)
    : BackgroundAnimation(window, num_of_flowers)
{
    for (int i = 0; i < m_num_of_objects; i++)
    {
        m_flowers.push_back(std::make_unique<Flower>(m_window));
    }
}

void Flowers::load_animation()
{
    for (auto &s : m_flowers)
    {
        s->setup(m_window);
    }
}

void Flowers::draw_animation()
{
    for (auto &s : m_flowers)
    {
        s->draw(m_window);
    }
}