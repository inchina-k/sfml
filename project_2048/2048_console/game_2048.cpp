#include "game_2048.hpp"
#include "../../libs/random.hpp"

using Random = effolkronium::random_static;

using namespace std;

Game::Game(int goal)
    : m_puzzle(4, vector<int>(4)), m_goal(goal), m_curr_score(0), m_game_won(false)
{
    add_random_number();
    add_random_number();
}

void Game::add_random_number()
{
    vector<pair<int, int>> free_cells;

    for (size_t i = 0; i < m_puzzle.size(); i++)
    {
        for (size_t j = 0; j < m_puzzle[i].size(); j++)
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

int Game::get_curr_score() const
{
    return m_curr_score;
}

int Game::get_at(int r, int c) const
{
    return m_puzzle[r][c];
}

int Game::get_goal() const
{
    return m_goal;
}

bool Game::game_won() const
{
    return m_game_won;
}

bool Game::filled_up() const
{
    for (const auto &row : m_puzzle)
    {
        for (auto num : row)
        {
            if (num == 0)
            {
                return false;
            }
        }
    }

    return true;
}

void Game::move_left()
{
    for (size_t h = 0; h < m_puzzle.size(); h++)
    {
        vector<bool> move(m_puzzle.size(), true);

        for (size_t i = 0; i < m_puzzle.size(); i++)
        {
            for (size_t j = i; j > 0 && move[j - 1]; j--)
            {
                if (m_puzzle[h][j] == m_puzzle[h][j - 1] && m_puzzle[h][j])
                {
                    m_puzzle[h][j - 1] *= 2;
                    m_puzzle[h][j] = 0;
                    move[j - 1] = false;

                    m_curr_score += m_puzzle[h][j - 1];

                    if (m_puzzle[h][j - 1] == m_goal)
                    {
                        m_game_won = true;
                    }

                    break;
                }
                else if (!m_puzzle[h][j - 1])
                {
                    m_puzzle[h][j - 1] = m_puzzle[h][j];
                    m_puzzle[h][j] = 0;
                }
            }
        }
    }
}

void Game::move_up()
{
    for (size_t h = 0; h < m_puzzle.size(); h++)
    {
        vector<bool> move(m_puzzle.size(), true);

        for (size_t j = 0; j < m_puzzle.size(); j++)
        {
            for (size_t i = j; i > 0 && move[i - 1]; i--)
            {
                if (m_puzzle[i][h] == m_puzzle[i - 1][h] && m_puzzle[i][h])
                {
                    m_puzzle[i - 1][h] *= 2;
                    m_puzzle[i][h] = 0;
                    move[i - 1] = false;

                    m_curr_score += m_puzzle[i - 1][h];

                    if (m_puzzle[i - 1][h] == m_goal)
                    {
                        m_game_won = true;
                    }

                    break;
                }
                else if (!m_puzzle[i - 1][h])
                {
                    m_puzzle[i - 1][h] = m_puzzle[i][h];
                    m_puzzle[i][h] = 0;
                }
            }
        }
    }
}

void Game::move_right()
{
    for (size_t h = 0; h < m_puzzle.size(); h++)
    {
        vector<bool> move(m_puzzle.size(), true);

        for (int i = m_puzzle.size() - 1; i >= 0; i--)
        {
            for (size_t j = i; j < m_puzzle.size() - 1 && move[j + 1]; j++)
            {
                if (m_puzzle[h][j] == m_puzzle[h][j + 1] && m_puzzle[h][j])
                {
                    m_puzzle[h][j + 1] *= 2;
                    m_puzzle[h][j] = 0;
                    move[j + 1] = false;

                    m_curr_score += m_puzzle[h][j + 1];

                    if (m_puzzle[h][j + 1] == m_goal)
                    {
                        m_game_won = true;
                    }

                    break;
                }
                else if (!m_puzzle[h][j + 1])
                {
                    m_puzzle[h][j + 1] = m_puzzle[h][j];
                    m_puzzle[h][j] = 0;
                }
            }
        }
    }
}

void Game::move_down()
{
    for (size_t h = 0; h < m_puzzle.size(); h++)
    {
        vector<bool> move(m_puzzle.size(), true);

        for (int j = m_puzzle.size() - 1; j >= 0; j--)
        {
            for (size_t i = j; i < m_puzzle.size() - 1 && move[i + 1]; i++)
            {
                if (m_puzzle[i][h] == m_puzzle[i + 1][h] && m_puzzle[i][h])
                {
                    m_puzzle[i + 1][h] *= 2;
                    m_puzzle[i][h] = 0;
                    move[i + 1] = false;

                    m_curr_score += m_puzzle[i + 1][h];

                    if (m_puzzle[i + 1][h] == m_goal)
                    {
                        m_game_won = true;
                    }

                    break;
                }
                else if (!m_puzzle[i + 1][h])
                {
                    m_puzzle[i + 1][h] = m_puzzle[i][h];
                    m_puzzle[i][h] = 0;
                }
            }
        }
    }
}