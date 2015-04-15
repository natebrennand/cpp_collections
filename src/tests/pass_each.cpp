#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {
  
    int sum = 0;

    auto ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5});

    ints.each([&](int x) { 
        sum += x; 
    });

    assert(sum == 15);
}
