#include <iostream>
#include <cmath>

using namespace std;

void print(double beg, double end, double step, double ptrToFunc(double x))
{
    double x = beg;

    while (x <= end)
    {
        cout << x << " " << ptrToFunc(x) << endl;
        x += step;
    }
}

int main()
{
    print(-5, 5, 1, abs);
    cout << endl;
    print(0, 3.14159265, 0.2, sin);
}