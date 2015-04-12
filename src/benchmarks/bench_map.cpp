#include <cassert>
#include <iostream>
#include <vector>

#include "../collections.h"
#include "benchmark.h"


int main() {
    // timing map on a vector of size 100,000
    auto input = Collection<int>::range(0, 100000);
    auto inc = [](int x) {return x+1;};

    auto result = bench([&](){
        return input.map(inc);
    });

    auto expectedOutput = Collection<int>::range(1, 100001);
    assert(result == Collection<int>(expectedOutput));
}
