#include "renderer.hpp"

#include <vector>

using namespace std;

Renderer::Renderer(Game &game, sf::Texture &texture, int x, int y, int num_of_cells, double cell_size, int image_size)
    : m_game(game), m_texture(texture), m_x(x), m_y(y), m_num_of_cells(num_of_cells), m_cell_size(cell_size), m_image_size(image_size)
{
    set_texture(m_texture);
}

void Renderer::set_texture(sf::Texture &texture)
{
    vector<int> nums = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    int left = 0, top = 0;

    if (!m_images.empty())
    {
        m_images.clear();
    }

    for (size_t i = 0; i < nums.size(); i++)
    {
        double sc = m_cell_size / m_image_size;

        sf::Sprite sprite(texture, sf::IntRect(left, top, m_image_size, m_image_size));
        sprite.scale(sc, sc);
        m_images.emplace(nums[i], sprite);

        left += m_image_size;
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

void Renderer::restart_game(sf::RenderWindow &window)
{
    m_game.update_puzzle();
    m_game.set_curr_score(0);
    m_game.set_game_started(false);
    m_game.set_win_status(false);

    m_game.add_random_number();
    m_game.add_random_number();

    render(window);
}