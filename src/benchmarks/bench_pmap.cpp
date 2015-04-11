#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

#include "../collections.h"


int main() {

    // timing concurrent map on a vector of size 100,000
    std::vector<int> input(100000);
    for (int i = 0; i < 100000; i++)
        input[i] = i;
    std::vector<int> output(100000);
    for (int i = 0; i < 100000; i++)
        output[i] = i + 1;

    auto incr = [](int x) {return x+1;};
    Collection<int> ints = Collection<int>(input);

    // concurrent map
    auto start = std::chrono::steady_clock::now();
    auto result = ints.pmap(incr, 10);
    auto diff = std::chrono::steady_clock::now() - start;

    assert(result == Collection<int>(output));

    std::cout << std::chrono::duration <double, std::milli> (diff).count() << " milliseconds" << std::endl;
}
