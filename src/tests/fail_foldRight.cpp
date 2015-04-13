#include <vector>
#include <iostream>
#include "../collections.h"


int main() {
    Collection<int> ints = Collection<int>(std::vector<int> {2, 3, 4, 5});

    bool wrong = [](bool x, bool y) {return x && y;};

    // foldRight should fail here because the function passed to it returns a
    // different type than the initial value that is passed to it

    int b = ints.foldRight(wrong, 1);
    std::cout << b << std::endl;
}

