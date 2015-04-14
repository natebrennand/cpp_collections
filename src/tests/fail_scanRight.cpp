#include <vector>
#include <iostream>
#include "../collections.h"

using namespace cpp_collections;

int main() {
    Collection<int> ints = Collection<int>(std::vector<int> {2, 3, 4, 5});

    int wrong = [](char x, int y) {return y;};

    int b = ints.scanRight(wrong, 'a');
    std::cout << b << std::endl;
}

