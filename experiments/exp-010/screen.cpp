#include "screen.hpp"

#include <iostream>

using namespace std;

Screen::Screen(int w, int h)
    : m_canvas(w, string(h, '-'))
{
}

void Screen::put(int x, int y)
{
    if (x >= 0 && y >= 0 && x < int(m_canvas.size()) && y < int(m_canvas.front().size()))
    {
        m_canvas[x][y] = '*';
    }
}

void Screen::show() const
{
    for (auto row : m_canvas)
    {
        for (auto e : row)
        {
            cout << e << ' ';
        }
        cout << endl;
    }
}