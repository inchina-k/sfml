#pragma once

#include "background_animations.hpp"
#include "flower.hpp"

#include <vector>
#include <memory>

class Flowers : public BackgroundAnimation
{
    std::vector<std::unique_ptr<Flower>> m_flowers;

public:
    Flowers(sf::RenderWindow &window, int num_of_flowers);

    void load_animation() override;

    void draw_animation() override;
};