#pragma once

#include <SFML/Graphics.hpp>

class Field
{
    int m_x;
    int m_y;
    int m_num_of_cells;
    sf::RectangleShape m_cell;

public:
    Field(int num_of_cells, int x, int y, double cell_size);

    void set_field_color(std::pair<sf::Color, sf::Color> &field_color);

    void draw_field(sf::RenderWindow &window);
};