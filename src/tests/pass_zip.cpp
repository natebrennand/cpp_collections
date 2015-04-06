#include <vector>
#include <tuple>
#include <cassert>

#include "../collections.h"


int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints1 = Collection<int>(std::vector<int> {1, 2, 3});

    auto ints2 = ints.zip(ints1);
    assert(ints2[0] == std::make_tuple(ints[0], ints1[0]));
    assert(ints2[1] == std::make_tuple(ints[1], ints1[1]));
    assert(ints2[2] == std::make_tuple(ints[2], ints1[2]));
    assert(ints2.size() == 3);
}
