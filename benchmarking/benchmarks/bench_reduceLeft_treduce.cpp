#include <iostream>
#include <vector>

#include "collections.h"
#include "benchmark.h"

#if defined COLLECTION_SIZE
#define size COLLECTION_SIZE
#else
#define size 100
#endif

#define trials 50

using namespace cpp_collections;


int main() {
    // timing reduce on a vector of size 100,000
    auto input = [](){ return range(0, size); };
    auto add = [](int x, int y) {return x+y;};
    random_generator rand_gen;

    auto rand_input = [&](){
        std::vector<int> data(size);
        for (int i = 0; i < size; i++)
            data[i] = rand_gen(size);
        return data;
    };

    std::cout << "Comparing reduceLeft & treduce"
        << "with size: " << size
        << ", and trials: " << trials << std::endl;


    bench(input, [&](Collection<int> i){
        return i.reduceLeft(add);
    }, trials, "reduceLeft: linear reduceLeft");

    bench(input, [&](Collection<int> i){
        return i.treduce(add, 4);
    }, trials, "treduce: parallel reduce");

    bench(input, [&](Collection<int> i){
        return i.preduce(add, 4);
    }, trials, "preduce: parallel reduce");


    bench(rand_input, [&](Collection<int> i){
        return i.reduceLeft(add);
    }, trials, "reduceLeft: linear reduceLeft w/ random data");

    bench(rand_input, [&](Collection<int> i){
        return i.treduce(add, 4);
    }, trials, "treduce: parallel reduce w/ random data");

    bench(rand_input, [&](Collection<int> i){
        return i.preduce(add, 4);
    }, trials, "preduce: parallel reduce w/ random data");
}
