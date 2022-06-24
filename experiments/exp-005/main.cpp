#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

using Grid = vector<vector<int>>;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML app");
    window.setVerticalSyncEnabled(true);

    Grid grid = {{2, 0, 4, 8}, {512, 16, 64, 2}, {0, 128, 0, 1024}, {2048, 256, 16, 32}};

    vector<int> numbers = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};

    unordered_map<int, sf::Sprite> images;

    sf::Texture nums_texture;
    nums_texture.loadFromFile("data/2-2048 pieces.png");

    const int cell_size = 100;

    int left = 0, top = 0;

    for (size_t i = 0; i < numbers.size(); i++)
    {
        images.emplace(numbers[i], sf::Sprite(nums_texture, sf::IntRect(left, top, cell_size, cell_size)));
        left += cell_size;
    }

    sf::RectangleShape cell(sf::Vector2f(cell_size, cell_size));
    cell.setFillColor(sf::Color(163, 160, 160));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color(209, 207, 207));

    double x = window.getSize().x / 2 - cell_size * 2;
    double y = window.getSize().y / 2 - cell_size * 2;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear();

        sf::Vector2f pos(x, y);

        for (auto &row : grid)
        {
            for (auto num : row)
            {
                cell.setPosition(pos.x, pos.y);
                window.draw(cell);

                images[num].setPosition(pos.x, pos.y);
                window.draw(images[num]);

                pos.x += cell_size;
            }

            pos.x = x;
            pos.y += cell_size;
        }

        window.display();
    }

#ifdef AUCA_DEBUG
    std::clog << "ok" << std::endl;
#endif
}
