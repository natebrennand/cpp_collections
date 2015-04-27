#include <cmath>
#include <ostream>
#include <vector>

#include "../cpp_collections.h"

using namespace cpp_collections;

int main() {

    // Collection syntax

    def_generator(fibs, int, int prev, int curr) {
        return Stream<int>(curr, [=]() { return fibs(curr, prev+curr); });
    };

    fibs(0, 1).take(10).print();

}
