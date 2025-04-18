#pragma once

#include <vector>
#include <map>
#include <queue>

class Game
{
    std::vector<std::vector<int>> m_puzzle;
    int m_goal;
    int m_curr_score;
    bool m_game_won;
    std::map<int, int> m_best_scores;
    std::queue<std::vector<std::vector<int>>> m_frames;

public:
    Game(int goal = 16);

    void add_random_number();

    void update_best_score();

    int get_curr_score() const;

    int get_at(int r, int c) const;

    int get_goal() const;

    int get_best_score(int goal) const;

    bool game_won() const;

    bool filled_up() const;

    bool merge_possible() const;

    void move_left();

    void move_up();

    void move_right();

    void move_down();

    bool frames_empty();

    std::vector<std::vector<int>> pop_frame();
};