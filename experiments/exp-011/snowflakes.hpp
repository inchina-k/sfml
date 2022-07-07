#pragma once

#include "background_animations.hpp"
#include "snowflake.hpp"

#include <vector>
#include <memory>

class Snowflakes : public BackgroundAnimation
{
    std::vector<std::unique_ptr<Snowflake>> m_snowflakes;

public:
    Snowflakes(sf::RenderWindow &window, int num_of_snowflakes);

    void load_animation() override;

    void draw_animation() override;
};