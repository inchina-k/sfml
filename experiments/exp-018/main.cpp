#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct B
{
    virtual void common_method() const = 0;
};

struct D1 : B
{
    void common_method() const override
    {
        cout << "D1::commom_method()" << endl;
    }

    void d1_method() const
    {
        cout << "D1::d1_method()" << endl;
    }
};

struct D2 : B
{
    void common_method() const override
    {
        cout << "D2::commom_method()" << endl;
    }

    void d2_method() const
    {
        cout << "D2::d2_method()" << endl;
    }
};

int main()
{
    vector<unique_ptr<B>> items;
    items.push_back(make_unique<D1>());
    items.push_back(make_unique<D2>());

    for (const auto &i : items)
    {
        i->common_method();

        if (auto i1 = dynamic_cast<D1 *>(i.get()))
        {
            i1->d1_method();
        }
        else if (auto i2 = dynamic_cast<D2 *>(i.get()))
        {
            i2->d2_method();
        }
    }
}