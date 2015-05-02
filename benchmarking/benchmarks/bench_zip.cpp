#include <iostream>
#include <vector>

#include "collections.h"
#include "benchmark.h"


#if defined COLLECTION_SIZE
#define csize COLLECTION_SIZE
#else
#define csize 100
#endif

#define trials 100

using namespace cpp_collections;


int main() {
    // timing map on a vector of size 100,000
    random_generator rand_gen;
    auto coll_input = [&]() { 
        std::vector<int> a(csize);
        std::vector<int> b(csize);
        std::vector<int> c(csize);
        for (int i = 0; i < csize; i++) {
            a[i] = rand_gen(csize);
            b[i] = rand_gen(csize);
            c[i] = rand_gen(csize);
        }
        return std::make_tuple(Collection<int>(a),Collection<int>(b),Collection<int>(c));
    };

    auto vec_input = [&]() {
        std::vector<int> a(csize);
        std::vector<int> b(csize);
        std::vector<int> c(csize);
        for (int i = 0; i < csize; i++) {
            a[i] = rand_gen(csize);
            b[i] = rand_gen(csize);
            c[i] = rand_gen(csize);
        }
        return std::make_tuple(a,b,c);
    };

    std::cout << "Comparing zip & make_tuple "
        << "with size: " << csize
        << ", and trials: " << trials << std::endl;

    bench(vec_input, [](std::tuple<std::vector<int>,std::vector<int>,std::vector<int>> t) {
        int dsize = std::min({std::get<0>(t).size(),std::get<1>(t).size(),std::get<2>(t).size()});
        std::vector<std::tuple<int,int,int>> data(dsize);
        for (int i = 0; i < dsize; i++)
            data[i] = std::make_tuple(std::get<0>(t)[i],std::get<1>(t)[i],std::get<2>(t)[i]);
        auto c = data;
    }, trials, "make_tuple of " + std::to_string(csize) + " with random data");

    bench(coll_input, [](std::tuple<Collection<int>,Collection<int>,Collection<int>> t) {
        auto c = zip(std::get<0>(t),std::get<1>(t),std::get<2>(t));
    }, trials, "zip of " + std::to_string(csize) + " with random data");
}

