#include "game_2048.hpp"
#include "../../libs/random.hpp"

using Random = effolkronium::random_static;

using namespace std;

Game::Game(int goal)
    : m_puzzle(4, vector<int>(4)), m_goal(goal), m_curr_score(0)
{
    add_random_number();
    add_random_number();
}

void Game::add_random_number()
{
    vector<pair<int, int>> free_cells;

    for (size_t i = 0; i < m_puzzle.size(); i++)
    {
        for (size_t j = 0; j < m_puzzle.size(); j++)
        {
            if (m_puzzle[i][j] == 0)
            {
                free_cells.emplace_back(i, j);
            }
        }
    }

    int num_coords = Random::get(0, int(free_cells.size() - 1));

    int x = free_cells[num_coords].first;
    int y = free_cells[num_coords].second;

    int probability = Random::get(0, 10);

    if (probability < 3)
    {
        m_puzzle[x][y] = 4;
    }
    else
    {
        m_puzzle[x][y] = 2;
    }
}

int Game::get_at(int r, int c) const
{
    return m_puzzle[r][c];
}