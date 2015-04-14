#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {
    auto even = [](int x) { return x % 2 == 0; };
    auto ints = Collection<int>(std::vector<int> {2, 3, 4, 5});

    auto ints2 = ints.filter(even);
    assert(ints2[0] == 2);
    assert(ints2[1] == 4);
}
