#include <vector>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;


int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints1 = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints2 = Collection<int>(std::vector<int> {1, 2, 3, 4});

    auto add = [](int x, int y, int z) { return x + y + z; };

    auto ints3 = zipWith(add, ints, ints1, ints2);
    assert(ints3[0] == 3);
    assert(ints3[1] == 6);
    assert(ints3[2] == 9);
    assert(ints3.size() == 3);
}
