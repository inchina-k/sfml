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

void move_left(Grid &grid)
{
    for (int h = 0; h < grid.size(); h++)
    {
        vector<bool> move(grid.size(), true);

        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = i; j > 0 && move[j - 1]; j--)
            {
                if (grid[h][j] == grid[h][j - 1] && grid[h][j])
                {
                    grid[h][j - 1] *= 2;
                    grid[h][j] = 0;
                    move[j - 1] = false;
                    break;
                }
                else if (!grid[h][j - 1])
                {
                    grid[h][j - 1] = grid[h][j];
                    grid[h][j] = 0;
                }
            }
        }
    }
}

void move_up(Grid &grid)
{
    for (int h = 0; h < grid.size(); h++)
    {
        vector<bool> move(grid.size(), true);

        for (int j = 0; j < grid.size(); j++)
        {
            for (int i = j; i > 0 && move[i - 1]; i--)
            {
                if (grid[i][h] == grid[i - 1][h] && grid[i][h])
                {
                    grid[i - 1][h] *= 2;
                    grid[i][h] = 0;
                    move[i - 1] = false;
                    break;
                }
                else if (!grid[i - 1][h])
                {
                    grid[i - 1][h] = grid[i][h];
                    grid[i][h] = 0;
                }
            }
        }
    }
}

void move_right(Grid &grid)
{
    for (int h = 0; h < grid.size(); h++)
    {
        vector<bool> move(grid.size(), true);

        for (int i = grid.size() - 1; i >= 0; i--)
        {
            for (int j = i; j < grid.size() - 1 && move[j + 1]; j++)
            {
                if (grid[h][j] == grid[h][j + 1] && grid[h][j])
                {
                    grid[h][j + 1] *= 2;
                    grid[h][j] = 0;
                    move[j + 1] = false;
                    break;
                }
                else if (!grid[h][j + 1])
                {
                    grid[h][j + 1] = grid[h][j];
                    grid[h][j] = 0;
                }
            }
        }
    }
}

void move_down(Grid &grid)
{
    for (int h = 0; h < grid.size(); h++)
    {
        vector<bool> move(grid.size(), true);

        for (int j = grid.size() - 1; j >= 0; j--)
        {
            for (int i = j; i < grid.size() - 1 && move[i + 1]; i++)
            {
                if (grid[i][h] == grid[i + 1][h] && grid[i][h])
                {
                    grid[i + 1][h] *= 2;
                    grid[i][h] = 0;
                    move[i + 1] = false;
                    break;
                }
                else if (!grid[i + 1][h])
                {
                    grid[i + 1][h] = grid[i][h];
                    grid[i][h] = 0;
                }
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