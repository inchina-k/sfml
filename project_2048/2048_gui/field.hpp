#pragma once

#include <SFML/Graphics.hpp>

class Field
{
    int m_x;
    int m_y;
    int m_num_of_cells;
    sf::RectangleShape m_cell;

public:
    Field(int num_of_cells, int x, int y, int cell_size);

    void draw_field(sf::RenderWindow &window);
};