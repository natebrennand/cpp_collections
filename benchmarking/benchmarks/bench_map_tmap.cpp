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
    auto inputVector = [](){
        std::vector<int> v(size);
        for (int i = 0; i < size; i++)
            v[i] = i;
        return v;
    };
    auto input = [](){ return range(0, size); };
    auto inc = [](int x) {return x+1;};
    random_generator rand_gen;

    auto rand_input = [&](){
        std::vector<int> data(size);
        for (int i = 0; i < size; i++)
            data[i] = rand_gen(size);
        return data;
    };

    std::cout << "Comparing map & tmap "
        << "with size: " << size
        << ", and trials: " << trials << std::endl;


    bench(inputVector, [&](std::vector<int> v) {
        for (int i = 0; i < size; i++)
            v[i] = v[i]+1;
        return v;
    }, trials, "for loop iteration");

    bench(input, [&](Collection<int> i) {
        return i.map(inc);
    }, trials, "map: linear map");

    bench(input, [&](Collection<int> i) {
        return i.tmap(inc, 4);
    }, trials, "tmap: parallel map");

    bench(input, [&](Collection<int> i) {
        return i.pmap(inc);
    }, trials, "pmap: linear map");


    bench(rand_input, [&](Collection<int> i) {
        return i.map(inc);
    }, trials, "map: linear map w/ random data");

    bench(rand_input, [&](Collection<int> i) {
        return i.tmap(inc, 4);
    }, trials, "tmap: parallel map w/ random data");

    bench(rand_input, [&](Collection<int> i) {
        return i.pmap(inc, 4);
    }, trials, "pmap: parallel map w/ random data");
}
