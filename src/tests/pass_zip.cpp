#include <vector>
#include <tuple>
#include <cassert>

#include "../collections.h"

int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints1 = Collection<int>(std::vector<int> {1, 2, 3});

    auto ints2 = Collection<int>::zip(ints, ints1);

    assert(ints2.size() == 3);
    assert(ints2[0] == std::make_tuple(ints[0], ints1[0]));
    assert(ints2[1] == std::make_tuple(ints[1], ints1[1]));
    assert(ints2[2] == std::make_tuple(ints[2], ints1[2]));


    // check that the shorter list size is used
    auto ints3 = Collection<int>(std::vector<int> {1, 2});
    auto ints4 = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints34 = Collection<int>::zip(ints3, ints4);
    assert(ints34[0] == std::make_tuple(1,1));
    assert(ints34[1] == std::make_tuple(2,2));
    assert(ints34.size() == 2);
}
