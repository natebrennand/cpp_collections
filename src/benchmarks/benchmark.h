#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>
#include <functional>


template<typename Func>
typename std::result_of<Func()>::type
bench(Func f){
    auto start = std::chrono::steady_clock::now();
    auto res = f();
    auto diff = std::chrono::steady_clock::now() - start;

    std::cout << std::chrono::duration <double, std::milli> (diff).count();
    std::cout << " milliseconds" << std::endl;

    return res;
};

#endif
