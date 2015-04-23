#include <cmath>
#include <ostream>
#include <vector>

#include "../collections.h"
#include "../streams.h"

using namespace cpp_collections;

int main() {

    auto isPrime = [](int x) { 
        return x >= 2 && range(2, ((int) sqrt(x)) + 1)
            .filter([=](int p) { return x % p == 0; })
            .size() == 0;
    };

    // standard syntax

    int primes = 0;
    int sum = 0;
    int i = 2;
    while (true) {
        if (isPrime(i)) {
            primes += 1;
            sum += i;
        }
        if (primes == 10000) {
            break;
        }
        i += 1;
    }

    // Collection syntax

    sum = from(2)
              .filter(isPrime)
              .take(10000)
              .reduceLeft([](int x, int y) {return x+y;});

}
