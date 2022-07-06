#include <iostream>
#include <memory>

#include "screen.hpp"
#include "rect.hpp"
#include "triangle.hpp"
#include "fstream"

using namespace std;

int main()
{
    Screen screen(20, 20);

    // Rect rect(2, 2, 6, 7);
    //  rect.draw(screen);

    // Triangle triangle(10, 10, 3);
    //  triangle.draw(screen);

    // cout << sizeof(rect) << endl;

    vector<unique_ptr<Shape>> shapes;

    shapes.push_back(make_unique<Rect>(2, 2, 6, 7));
    shapes.push_back(make_unique<Triangle>(10, 10, 3));

    fstream fout("shapes.txt");

    for (const auto &p : shapes)
    {
        p->draw(screen);
        p->save(fout);
    }

    screen.show();

    // for (auto p : shapes)
    // {
    //     delete p;
    // }
}