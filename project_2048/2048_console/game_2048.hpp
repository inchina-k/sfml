#pragma once

#include <vector>

class Game
{
    std::vector<std::vector<int>> m_puzzle;
    int m_goal;
    int m_curr_score;
    std::vector<int> m_score;

public:
    Game(int goal = 16);

    void add_random_number();

    int get_at(int r, int c) const;
};