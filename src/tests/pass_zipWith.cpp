#include <vector>
#include <cassert>

#include "../collections.h"


int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints1 = Collection<int>(std::vector<int> {1, 2, 3});

    auto add = [](int x, int y) { return x + y; };

    auto ints2 = ints.zipWith(add, ints1);
    assert(ints2[0] == 2);
    assert(ints2[1] == 4);
    assert(ints2[2] == 6);
    assert(ints2.size() == 3);
}
