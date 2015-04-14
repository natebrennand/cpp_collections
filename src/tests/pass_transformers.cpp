#include <list>
#include <array>
#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {
    std::vector<int> int_vector {2, 3, 4, 5};
    auto ints = Collection<int>(int_vector);
    int sum = 0;

    std::array<int, 4> int_array {2, 3, 4, 5};

    std::vector<int> vec = ints.vector();
    assert(vec[0] == 2);
    assert(vec[3] == 5);
    assert(vec.size() == 4);

    std::list<int> lst = ints.list();
    for (auto i : lst)
        sum += i;
    assert(sum == 14);
    assert(lst.size() == 4);
}
