#include <cmath>
#include <ostream>
#include <vector>

#include "../cpp_collections.h"

using namespace cpp_collections;

int main() {

    // Collection syntax

    auto fibs = recurrence([](std::tuple<int,int> t) {
        return std::get<0>(t) + std::get<1>(t);
    }, std::make_tuple(0, 1));

    fibs.take(10).print();

}
