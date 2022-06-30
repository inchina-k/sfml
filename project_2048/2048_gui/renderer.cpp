#include "renderer.hpp"

#include <vector>

using namespace std;

Renderer::Renderer(Game &game, sf::Texture nums_texture, int x, int y, int num_of_cells, int cell_size)
    : m_game(game), m_texture(nums_texture), m_x(x), m_y(y), m_num_of_cells(num_of_cells), m_cell_size(cell_size)
{
    vector<int> nums = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    int left = 0, top = 0;

    for (size_t i = 0; i < nums.size(); i++)
    {
        m_images.emplace(nums[i], sf::Sprite(m_texture, sf::IntRect(left, top, m_cell_size, m_cell_size)));
        left += m_cell_size;
    }
}

void Renderer::render(sf::RenderWindow &window)
{
    vector<vector<int>> curr_frame = m_game.get_next_frame();

    sf::Vector2f pos(m_x, m_y);

    for (int i = 0; i < m_num_of_cells; i++)
    {
        for (int j = 0; j < m_num_of_cells; j++)
        {
            int num = curr_frame[i][j];

            m_images[num].setPosition(pos.x, pos.y);
            window.draw(m_images[num]);

            pos.x += m_cell_size;
        }

        pos.x = m_x;
        pos.y += m_cell_size;
    }
}

void Renderer::restart_game(int goal, sf::RenderWindow &window)
{
    m_game.update_puzzle();
    m_game.set_goal(goal);
    m_game.set_curr_score(0);
    m_game.set_win_status(false);

    m_game.add_random_number();
    m_game.add_random_number();

    render(window);
}