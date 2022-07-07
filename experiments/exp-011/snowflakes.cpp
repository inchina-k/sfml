#include "snowflakes.hpp"

Snowflakes::Snowflakes(sf::RenderWindow &window, int num_of_snowflakes)
    : BackgroundAnimation(window, num_of_snowflakes)
{
    for (int i = 0; i < m_num_of_objects; i++)
    {
        m_snowflakes.push_back(std::make_unique<Snowflake>(m_window));
    }
}

void Snowflakes::load_animation()
{
    for (auto &s : m_snowflakes)
    {
        s->setup(m_window);
    }
}

void Snowflakes::draw_animation()
{
    for (auto &s : m_snowflakes)
    {
        s->draw(m_window);
    }
}