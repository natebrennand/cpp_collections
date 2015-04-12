#include <cassert>
#include <iostream>
#include <vector>

#include "../collections.h"
#include "benchmark.h"


int main() {
    // timing concurrent map on a vector of size 100,000
    auto input = Collection<int>::range(0, 100000);
    auto incr = [](int x) {return x+1;};

    // concurrent map
    auto result = bench([&](){
        return input.pmap(incr, 10);
    });

    auto output = Collection<int>::range(1, 100001).vector();
    assert(result == Collection<int>(output));
}
