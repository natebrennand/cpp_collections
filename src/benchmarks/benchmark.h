#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <iomanip>


// random_generator creates a functor for generation of a random number in a range.
class random_generator{
    std::mt19937 mt;
public:
    int operator()(int range){ return std::uniform_int_distribution<int> (0, range-1)(mt); }
    random_generator(){ mt = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()); }
};


// bench wraps a function and it's input generator to benchmark the performance.
// g: a generator that returns the input for function, f
// f: a function that is being benchmarked
// trials: the number of trials to run
// name: a short name of the test being run
template<typename Gen, typename Func>
void
bench(Gen g, Func f, int trials, std::string name){
    typedef std::chrono::duration<double, std::milli> time;
    time total;

    for (int i = 0; i < trials; i++) {
        auto input = g();

        auto start = std::chrono::steady_clock::now();
        f(input);
        auto end = std::chrono::steady_clock::now();

        total = total + (end - start);
    }

    std::cout << "\t" << std::setprecision(5) << total.count()/trials << " milliseconds | ";
    std::cout << name;
    std::cout << " | " << trials << " trials" << std::endl;
};

#endif
