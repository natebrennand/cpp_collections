#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"


int main() {
    auto sub = [](int x, int y) {return x-y;};

    auto ints2 = Collection<int>(std::vector<int> {1, 2, 3, 4});

    auto i = ints2.foldRight(sub, 5);
    assert(i == -5);
}
