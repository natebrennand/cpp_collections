#include <vector>
#include <iostream>
#include "../collections.h"

using namespace cpp_collections;

int main() {
    Collection<int> ints = Collection<int>(std::vector<int> {2, 3, 4, 5});

    bool wrong = [](bool x, bool y) {return x && y;};

    // foldLeft should fail here because the function passed to it returns a
    // different type than the initial value that is passed to it

    int b = ints.foldLeft(wrong, 1);
    std::cout << b << std::endl;
}

