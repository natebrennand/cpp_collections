#include <vector>
#include <cassert>

#include "../collections.h"


int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5});

    assert(ints.head() == 1);
    assert(ints.last() == 5);
    assert(ints.init() == Collection<int>(std::vector<int> {1, 2, 3, 4}));
    assert(ints.tail() == Collection<int>(std::vector<int> {2, 3, 4, 5}));
}
