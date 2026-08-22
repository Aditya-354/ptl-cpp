#include <iostream>
// #include <print>
#include "../ptl/list.hpp"

auto test() -> void {
    std::cout << "Testing " << __PRETTY_FUNCTION__ << ":\n";
    ptl::LinkedList<int> list1  = {1, 2, 3, 4, 5, 6};
    ptl::LinkedList<int> list2 {2, 4, 5};
    list2 = std::move(list1);
    ptl::LinkedList<int> list3;
    list3 = std::move(list2);

    const auto list_find {
        [](ptl::LinkedList<int>& list, const int& el) -> ptl::LinkedList<int>::iterator
        {
            for (ptl::LinkedList<int>::iterator it {list.begin()};
                    it != list.end(); ++it)
                if (it->data == el) return it;
            return list.end();
        }
    };

    ptl::LinkedList<int>::iterator it {list_find(list3, 4)};
    list3.insert(it, 25);

    for (const auto& o : list3)
        std::cout << o << '\n';
}

auto main() -> int
{
    test();
    return 0;
}
