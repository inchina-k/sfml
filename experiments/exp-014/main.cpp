#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class Button
{
    int m_left_upper_x;
    int m_left_upper_y;
    int m_w;
    int m_h;
    void (*ptr_to_f)();

public:
    Button(int lux, int luy, int w, int h, void ptf())
        : m_left_upper_x(lux), m_left_upper_y(luy), m_w(w), m_h(h), ptr_to_f(ptf)
    {
    }

    bool contains(int x, int y) const
    {
        return m_left_upper_x >= x && x <= m_left_upper_x + m_w &&
               m_left_upper_y >= y && y <= m_left_upper_y + m_h;
    }

    void run() const
    {
        ptr_to_f();
    }
};

void print_something()
{
    cout << "These are ten digits:" << endl;
}

void print_nums()
{
    for (int i = 0; i < 10; i++)
    {
        cout << i << ' ';
    }
    cout << endl;
}

void print_mess()
{
    cout << "That's it. Nothing more to happen." << endl;
}

int main()
{
    vector<Button> buttons = {Button(15, 15, 10, 10, print_something),
                              Button(50, 50, 10, 10, print_nums),
                              Button(70, 80, 5, 5, print_mess)};

    for (int x, y; cin >> x >> y;)
    {
        for (const Button &b : buttons)
        {
            if (b.contains(x, y))
            {
                b.run();
            }
        }
    }
}