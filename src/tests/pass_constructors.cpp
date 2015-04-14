#include <list>
#include <array>
#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {
    auto even = [](int x) { return x % 2 == 0; };

    std::vector<int> int_vector {2, 3, 4, 5};
    std::list<int> int_list {2, 3, 4, 5};
    std::array<int, 4> int_array {2, 3, 4, 5};
    int int_c_array[] {2, 3, 4, 5};

    auto ints = Collection<int>(int_vector);
    assert(ints[0] == 2);
    ints = Collection<int>(int_list);
    assert(ints[1] == 3);
    ints = Collection<int>(int_array);
    assert(ints[2] == 4);
    ints = Collection<int>(int_c_array, 4);
    assert(ints[3] == 5);

}
