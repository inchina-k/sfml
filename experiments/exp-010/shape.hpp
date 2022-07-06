#pragma once

#include "screen.hpp"
#include <iostream>
#include <ostream>

class Shape
{
protected:
    int m_x, m_y;

public:
    Shape(int x, int y)
        : m_x(x), m_y(y) {}

    virtual ~Shape()
    {
        std::cout << "~Shape()" << std::endl;
    }

    virtual void draw(Screen &screen) const = 0;

    virtual void save(std::ostream &os) const = 0;
};