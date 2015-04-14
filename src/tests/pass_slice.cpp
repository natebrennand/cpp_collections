#include <vector>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5});

    auto ints2 = ints.slice(2,4);
    assert(ints2[0] == 3);
    assert(ints2[1] == 4);
    assert(ints2.size() == 2);
}
