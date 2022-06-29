#include "field.hpp"

Field::Field(int x, int y, int num_of_cells, int cell_size)
    : m_x(x), m_y(y), m_num_of_cells(num_of_cells), m_cell(sf::Vector2f(cell_size, cell_size))
{
    m_cell.setFillColor(sf::Color(189, 203, 219));
    m_cell.setOutlineThickness(1);
    m_cell.setOutlineColor(sf::Color(225, 237, 252));
}

void Field::draw_field(sf::RenderWindow &window)
{
    sf::Vector2f pos(m_x, m_y);

    for (int i = 0; i < m_num_of_cells; i++)
    {
        for (int j = 0; j < m_num_of_cells; j++)
        {
            m_cell.setPosition(pos.x, pos.y);
            window.draw(m_cell);

            pos.x += m_cell.getSize().x;
        }

        pos.x = m_x;
        pos.y += m_cell.getSize().x;
    }
}