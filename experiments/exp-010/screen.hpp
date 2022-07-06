#pragma once

#include <vector>
#include <string>

class Screen
{
    std::vector<std::string> m_canvas;

public:
    Screen(int w, int h);

    void put(int x, int y);

    void show() const;
};