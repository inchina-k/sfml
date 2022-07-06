#pragma once

#include "shape.hpp"

class Rect : public Shape
{
    int m_w, m_h;

public:
    Rect(int x, int y, int w, int h);

    ~Rect();

    void draw(Screen &screen) const override;

    void save(std::ostream &os) const override;
};