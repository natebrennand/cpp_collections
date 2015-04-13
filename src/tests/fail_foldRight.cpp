#include <vector>
#include <iostream>
#include "../collections.h"


int main() {
    Collection<int> ints = Collection<int>(std::vector<int> {2, 3, 4, 5});

    bool wrong = [](bool x, bool y) {return x && y;};

    int b = ints.foldRight(wrong, 1);
    std::cout << b << std::endl;
}

