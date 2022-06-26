#pragma once

#include <vector>

class Game
{
    std::vector<std::vector<int>> m_puzzle;
    int m_goal;
    int m_curr_score;
    bool m_game_won;
    // std::vector<int> m_scores;

public:
    Game(int goal = 16);

    void add_random_number();

    int get_curr_score() const;

    int get_at(int r, int c) const;

    int get_goal() const;

    bool game_won() const;

    bool filled_up() const;

    void move_left();

    void move_up();

    void move_right();

    void move_down();
};