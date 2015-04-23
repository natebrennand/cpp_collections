#include <cmath>
#include <ostream>
#include <vector>

#include "../collections.h"
#include "../streams.h"

using namespace cpp_collections;

int main() {

    // Collection syntax

    std::function<Stream<int>(int, int)> fib = [&](int prev, int cur) -> Stream<int> {
        return Stream<int>(prev + cur, [=]() -> Stream<int> {
            return fib(cur, prev + cur);
        });
    };

    auto fibs = Stream<int>(1, [=]() -> Stream<int> {
        return fib(0, 1);
    });

    fibs.take(10).print();

}
