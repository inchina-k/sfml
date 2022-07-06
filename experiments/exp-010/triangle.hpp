#pragma once

#include "shape.hpp"

class Triangle : public Shape
{
    int m_h;

public:
    Triangle(int x, int y, int h);

    ~Triangle();

    void draw(Screen &screen) const override;

    void save(std::ostream &os) const override;
};