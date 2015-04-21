#include <cmath>
#include <ostream>
#include <vector>

#include "../collections.h"
#include "../streams.h"

using namespace cpp_collections;

int main() {

    // Collection syntax
    auto isPrime = [](int x) { 
        return x >= 2 && range(2, ((int) sqrt(x)) + 1)
            .filter([=](int p) { return x % p == 0; })
            .size() == 0;
    };

    // infinite stream of primes
    auto primesI = from(2).filter(isPrime);

    // sum first 10000 primes
    int sum = primesI.take(10000).reduceLeft([](int x, int y) {return x+y;});

}
