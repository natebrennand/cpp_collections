#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"


int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5});

    auto ints2 = ints.range(2,4);
    assert(ints2.Data[0] == 2);
    assert(ints2.Data[1] == 3);
    assert(ints2.Data[2] == 4);
}
