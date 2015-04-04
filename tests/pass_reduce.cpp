#include <vector>
#include <iostream>

#include "../collections.h"


int main() {
    auto add = [](int x, int y) {return x+y;};

    auto ints2 = Collection<int>(std::vector<int> {2, 3, 4, 5});

    auto i = ints2.reduce(add, 1);
    std::cout << i << std::endl;
}
