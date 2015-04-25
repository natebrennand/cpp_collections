#include <cmath>
#include <ostream>
#include <vector>

#include "../cpp_collections.h"

using namespace cpp_collections;

int main() {

    // Collection syntax

    def_generator(fib, int, int prev, int cur) {
        return Stream<int>(prev + cur, [=]() -> Stream<int> {
            return fib(cur, prev + cur);
        });
    };

    auto fibs = Stream<int>(1, [=]() -> Stream<int> {
        return fib(0, 1);
    });

    fibs.take(10).print();

}
