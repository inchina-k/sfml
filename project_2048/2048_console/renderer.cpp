#include "renderer.hpp"

#include <iostream>
#include <iomanip>

using namespace std;

void Renderer::render() const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << setw(5) << m_game.get_at(i, j);
        }
        cout << endl;
    }
}