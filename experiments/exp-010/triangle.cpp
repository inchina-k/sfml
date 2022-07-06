#include "triangle.hpp"

Triangle::Triangle(int x, int y, int h)
    : Shape(x, y), m_h(h)
{
}

Triangle::~Triangle()
{
    std::cout << "~Triangle()" << std::endl;
}

void Triangle::draw(Screen &screen) const
{
    int length = 1;

    for (int row = m_y; row < m_y + m_h; row++)
    {
        for (int col = m_x; col < m_x + length; col++)
        {
            screen.put(row, col - length / 2);
        }

        length += 2;
    }
}

void Triangle::save(std::ostream &os) const
{
    os << "Triangle: " << m_x << ' ' << m_y << ", " << m_h << std::endl;
}