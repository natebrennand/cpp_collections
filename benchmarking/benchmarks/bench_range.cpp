#include <iostream>
#include <vector>

#include "../../src/cpp_collections.h"
#include "benchmark.h"



#if defined COLLECTION_SIZE
#define size COLLECTION_SIZE
#else
#define size 100
#endif


#define trials 100

using namespace cpp_collections;


int main() {
    // timing map on a vector of size 100,000
    auto input = [](){ return size; };


    std::cout << "Comparing range & for loops"
        << "with size: " << size
        << ", and trials: " << trials << std::endl;

    bench(input, [](int n) {
        std::vector<int> data(n);
        for (int i = 0; i < n; i++)
            data[i] = i;
        auto c = data;
    }, trials, "for loop range of " + std::to_string(size));

    bench(input, [](int i) {
        auto c = range(i);
    }, trials, "range of " + std::to_string(size));
}

