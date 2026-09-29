#include <iostream>
#include "list.hpp"

int main()
{
    ptl::List<int> l1 {};
    l1.insert(l1.begin(), 1);
    std::cout << l1.front() << '\n';
    l1.pop_back();

    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.push_back(4);
    l1.push_back(5);
    for (ptl::List<int>::iterator it {l1.begin()}; it != l1.end(); it++)
        std::cout << *it << '\n';
    l1.clear();
    return 0;
}
