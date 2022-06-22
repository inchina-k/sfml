#include <iostream>
#include <vector>

using namespace std;
using Grid = vector<vector<int>>;

enum class Direction
{
    Left,
    Up,
    Right,
    Down
};

void read_grid(Grid &grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid.front().size(); j++)
        {
            cin >> grid[i][j];
        }
    }
}

bool same_number(int num_1, int num_2)
{
    return num_1 == num_2;
}

void move_left(Grid &grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = grid.size() - 1; j > 0; j--)
        {
            if (same_number(grid[i][j], grid[i][j - 1]))
            {
                grid[i][j - 1] *= 2;
                grid[i][j] = 0;
                break;
            }
            else if (grid[i][j - 1] == 0)
            {
                grid[i][j - 1] = grid[i][j];
                grid[i][j] = 0;
            }
        }
    }
}

void move_up(Grid &grid)
{
    for (int j = 0; j < grid.size(); j++)
    {
        for (int i = grid.size() - 1; i > 0; i--)
        {
            if (same_number(grid[i][j], grid[i - 1][j]))
            {
                grid[i - 1][j] *= 2;
                grid[i][j] = 0;
                break;
            }
            else if (grid[i - 1][j] == 0)
            {
                grid[i - 1][j] = grid[i][j];
                grid[i][j] = 0;
            }
        }
    }
}

void move_right(Grid &grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid.size() - 1; j++)
        {
            if (same_number(grid[i][j], grid[i][j + 1]))
            {
                grid[i][j + 1] *= 2;
                grid[i][j] = 0;
                break;
            }
            else if (grid[i][j + 1] == 0)
            {
                grid[i][j + 1] = grid[i][j];
                grid[i][j] = 0;
            }
        }
    }
}

void move_down(Grid &grid)
{
    for (int j = 0; j < grid.size(); j++)
    {
        for (int i = 0; i < grid.size() - 1; i++)
        {
            if (same_number(grid[i][j], grid[i + 1][j]))
            {
                grid[i + 1][j] *= 2;
                grid[i][j] = 0;
            }
            else if (grid[i + 1][j] == 0)
            {
                grid[i + 1][j] = grid[i][j];
                grid[i][j] = 0;
            }
        }
    }
}

void show_grid(Grid &grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        bool is_first = true;

        for (int j = 0; j < grid.front().size(); j++)
        {
            if (!is_first)
            {
                cout << ' ';
            }
            cout << grid[i][j];

            is_first = false;
        }

        cout << endl;
    }
}

int main()
{
    const int n_of_cells = 4;

    Grid grid(n_of_cells, vector<int>(n_of_cells));

    read_grid(grid);

    int direction;
    cin >> direction;

    switch (direction)
    {
    case static_cast<int>(Direction::Left):
        move_left(grid);
        break;
    case static_cast<int>(Direction::Up):
        move_up(grid);
        break;
    case static_cast<int>(Direction::Right):
        move_right(grid);
        break;
    case static_cast<int>(Direction::Down):
        move_down(grid);
        break;
    }

    show_grid(grid);
}