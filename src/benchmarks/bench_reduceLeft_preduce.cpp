#include <iostream>
#include <vector>

#include "../collections.h"
#include "benchmark.h"


#define size 100000
#define trials 100


int main() {
    // timing reduce on a vector of size 100,000
    auto input = [](){ return Collection<int>::range(0, size); };
    auto add = [](int x, int y) {return x+y;};
    random_generator rand_gen;

    auto rand_input = [&](){
        std::vector<int> data(size);
        for (int i = 0; i < size; i++)
            data[i] = rand_gen(size);
        return data;
    };

    std::cout << "Comparing reduceLeft & preduce" << std::endl;


    bench(input, [&](Collection<int> i){
        return i.reduceLeft(add);
    }, trials, "reduceLeft: linear reduceLeft");

    bench(input, [&](Collection<int> i){
        return i.preduce(add, 10);
    }, trials, "preduce: parallel reduce");

    bench(rand_input, [&](Collection<int> i){
        return i.reduceLeft(add);
    }, trials, "reduceLeft: linear reduceLeft w/ random data");

    bench(rand_input, [&](Collection<int> i){
        return i.preduce(add, 10);
    }, trials, "preduce: parallel reduce w/ random data");
}
