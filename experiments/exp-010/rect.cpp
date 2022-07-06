#include "rect.hpp"

Rect::Rect(int x, int y, int w, int h)
    : Shape(x, y), m_w(w), m_h(h)
{
}

Rect::~Rect()
{
    std::cout << "~Rect()" << std::endl;
}

void Rect::draw(Screen &screen) const
{
    for (int i = m_x; i < m_x + m_w; i++)
    {
        for (int j = m_y; j < m_y + m_h; j++)
        {
            screen.put(i, j);
        }
    }
}

void Rect::save(std::ostream &os) const
{
    os << "Rect: " << m_x << ' ' << m_y << ", " << m_w << ' ' << m_y << std::endl;
}