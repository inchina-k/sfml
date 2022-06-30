#include "game_2048.hpp"
#include "../../libs/random.hpp"

#include <fstream>

using Random = effolkronium::random_static;

using namespace std;

Game::Game(int goal)
    : m_puzzle(4, vector<int>(4)), m_goal(goal), m_curr_score(0), m_game_won(false)
{
    vector<int> nums = {16, 32, 64, 128, 256, 512, 1024, 2048};

    fstream finp("best.data");
    int num;
    for (auto e : nums)
    {
        finp >> num;
        m_best_scores.emplace(e, num);
    }
    finp.close();

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

    if (!free_cells.empty())
    {
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
}

void Game::update_best_score()
{
    ofstream fout("best.data");
    for (const auto &e : m_best_scores)
    {
        fout << e.second << endl;
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

int Game::get_best_score(int goal) const
{
    return m_best_scores.find(goal)->second;
}

void Game::update_puzzle()
{
    for (auto &row : m_puzzle)
    {
        for (auto &num : row)
        {
            num = 0;
        }
    }
}

void Game::set_goal(int goal)
{
    m_goal = goal;
}

void Game::set_curr_score(int curr_score)
{
    m_curr_score = curr_score;
}

void Game::set_win_status(bool game_won)
{
    m_game_won = game_won;
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

bool Game::merge_possible() const
{
    for (int i = 0; i < int(m_puzzle.size()); i++)
    {
        for (int j = 0; j < int(m_puzzle[i].size()); j++)
        {
            int curr = m_puzzle[i][j];

            if ((j - 1 >= 0 && curr == m_puzzle[i][j - 1]) ||
                (i - 1 >= 0 && curr == m_puzzle[i - 1][j]) ||
                (j + 1 < int(m_puzzle[i].size()) && curr == m_puzzle[i][j + 1]) ||
                (i + 1 < int(m_puzzle.size()) && curr == m_puzzle[i + 1][j]))
            {
                return true;
            }
        }
    }

    return false;
}

void Game::move_left()
{
    vector<vector<bool>> merged(m_puzzle.size(), vector<bool>(m_puzzle.size()));

    for (int step = 0; step < 3; step++)
    {
        bool is_changed = false;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 1; col < 4; col++)
            {
                if (m_puzzle[row][col] == m_puzzle[row][col - 1] && m_puzzle[row][col] && !merged[row][col - 1] && !merged[row][col])
                {
                    m_puzzle[row][col - 1] *= 2;
                    m_puzzle[row][col] = 0;
                    merged[row][col - 1] = true;

                    m_curr_score += m_puzzle[row][col - 1];
                    m_best_scores[m_goal] = max(m_best_scores[m_goal], m_curr_score);

                    if (m_puzzle[row][col - 1] == m_goal)
                    {
                        m_game_won = true;
                    }

                    is_changed = true;
                }
                else if (!m_puzzle[row][col - 1] && m_puzzle[row][col])
                {
                    m_puzzle[row][col - 1] = m_puzzle[row][col];
                    m_puzzle[row][col] = 0;
                    is_changed = true;
                }
            }
        }

        if (is_changed)
        {
            m_frames.push(m_puzzle);
        }
    }

    add_random_number();
}

void Game::move_up()
{
    vector<vector<bool>> merged(m_puzzle.size(), vector<bool>(m_puzzle.size()));

    for (int step = 0; step < 3; step++)
    {
        bool is_changed = false;

        for (int row = 1; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                if (m_puzzle[row][col] == m_puzzle[row - 1][col] && m_puzzle[row][col] && !merged[row - 1][col] && !merged[row][col])
                {
                    m_puzzle[row - 1][col] *= 2;
                    m_puzzle[row][col] = 0;
                    merged[row - 1][col] = true;

                    m_curr_score += m_puzzle[row - 1][col];
                    m_best_scores[m_goal] = max(m_best_scores[m_goal], m_curr_score);

                    if (m_puzzle[row - 1][col] == m_goal)
                    {
                        m_game_won = true;
                    }

                    is_changed = true;
                }
                else if (!m_puzzle[row - 1][col] && m_puzzle[row][col])
                {
                    m_puzzle[row - 1][col] = m_puzzle[row][col];
                    m_puzzle[row][col] = 0;
                    is_changed = true;
                }
            }
        }

        if (is_changed)
        {
            m_frames.push(m_puzzle);
        }
    }

    add_random_number();
}

void Game::move_right()
{
    vector<vector<bool>> merged(m_puzzle.size(), vector<bool>(m_puzzle.size()));

    for (int step = 0; step < 3; step++)
    {
        bool is_changed = false;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 2; col >= 0; col--)
            {
                if (m_puzzle[row][col] == m_puzzle[row][col + 1] && m_puzzle[row][col] && !merged[row][col + 1] && !merged[row][col])
                {
                    m_puzzle[row][col + 1] *= 2;
                    m_puzzle[row][col] = 0;
                    merged[row][col + 1] = true;

                    m_curr_score += m_puzzle[row][col + 1];
                    m_best_scores[m_goal] = max(m_best_scores[m_goal], m_curr_score);

                    if (m_puzzle[row][col + 1] == m_goal)
                    {
                        m_game_won = true;
                    }

                    is_changed = true;
                }
                else if (!m_puzzle[row][col + 1] && m_puzzle[row][col])
                {
                    m_puzzle[row][col + 1] = m_puzzle[row][col];
                    m_puzzle[row][col] = 0;
                    is_changed = true;
                }
            }
        }

        if (is_changed)
        {
            m_frames.push(m_puzzle);
        }
    }

    add_random_number();
}

void Game::move_down()
{
    vector<vector<bool>> merged(m_puzzle.size(), vector<bool>(m_puzzle.size()));

    for (int step = 0; step < 3; step++)
    {
        bool is_changed = false;

        for (int row = 2; row >= 0; row--)
        {
            for (int col = 0; col < 4; col++)
            {
                if (m_puzzle[row][col] == m_puzzle[row + 1][col] && m_puzzle[row][col] && !merged[row + 1][col] && !merged[row][col])
                {
                    m_puzzle[row + 1][col] *= 2;
                    m_puzzle[row][col] = 0;
                    merged[row + 1][col] = true;

                    m_curr_score += m_puzzle[row + 1][col];
                    m_best_scores[m_goal] = max(m_best_scores[m_goal], m_curr_score);

                    if (m_puzzle[row + 1][col] == m_goal)
                    {
                        m_game_won = true;
                    }

                    is_changed = true;
                }
                else if (!m_puzzle[row + 1][col] && m_puzzle[row][col])
                {
                    m_puzzle[row + 1][col] = m_puzzle[row][col];
                    m_puzzle[row][col] = 0;
                    is_changed = true;
                }
            }
        }

        if (is_changed)
        {
            m_frames.push(m_puzzle);
        }
    }

    add_random_number();
}

vector<vector<int>> Game::pop_frame()
{
    vector<vector<int>> curr_frame = m_frames.front();
    m_frames.pop();
    return curr_frame;
}

bool Game::frames_empty()
{
    return m_frames.empty();
}

std::vector<std::vector<int>> Game::get_next_frame()
{
    if (!frames_empty())
    {
        return pop_frame();
    }

    return m_puzzle;
}