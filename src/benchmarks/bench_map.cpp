#include <cassert>
#include <iostream>
#include <vector>

#include "../collections.h"
#include "benchmark.h"


int main() {
    // timing map on a vector of size 100,000
    std::vector<int> input(100000);
    for (int i = 0; i < 100000; i++)
        input[i] = i;

    auto inc = [](int x) {return x+1;};
    Collection<int> ints = Collection<int>(input);

    auto result = bench([&](){
        return ints.map(inc);
    });


    std::vector<int> expectedOutput(100000);
    for (int i = 0; i < 100000; i++)
        expectedOutput[i] = i + 1;

    assert(result == Collection<int>(expectedOutput));
}
