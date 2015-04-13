#include <iostream>
#include <vector>

#include "../collections.h"
#include "benchmark.h"


#define size1 100
#define size2 100000
#define trials 100


int main() {
    // timing map on a vector of size 100,000
    auto input1 = [](){ return size1; };
    auto input2 = [](){ return size2; };


    std::cout << "Comparing range & for loops" << std::endl;


    bench(input1, [&](int i) {
        return Collection<int>::range(i).vector();
    }, trials, "range of " + std::to_string(size1));

    bench(input1, [&](int n) {
        std::vector<int> data(n);
        for (int i = 0; i < n; i++)
            data[i] = i;
        return data;
    }, trials, "range of " + std::to_string(size1));

    bench(input2, [&](int i) {
        return Collection<int>::range(i).vector();
    }, trials, "range of " + std::to_string(size2));

    bench(input2, [&](int n) {
        std::vector<int> data(n);
        for (int i = 0; i < n; i++)
            data[i] = i;
        return data;
    }, trials, "for loop range of " + std::to_string(size2));
}

