#include <cassert>
#include <iostream>
#include <vector>

#include "../collections.h"
#include "benchmark.h"


int main() {
    // timing concurrent map on a vector of size 100,000
    std::vector<int> input(100000);
    for (int i = 0; i < 100000; i++)
        input[i] = i;

    auto incr = [](int x) {return x+1;};
    Collection<int> ints = Collection<int>(input);

    // concurrent map
    auto result = bench([&](){
        return ints.pmap(incr, 10);
    });

    std::vector<int> output(100000);
    for (int i = 0; i < 100000; i++)
        output[i] = i + 1;
    assert(result == Collection<int>(output));
}
