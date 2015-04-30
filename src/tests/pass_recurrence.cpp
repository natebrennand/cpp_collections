#include <cmath>
#include <ostream>
#include <vector>
#include <cassert>

#include "../cpp_collections.h"

using namespace cpp_collections;

int main() {

    auto fibs = recurrence([](std::tuple<int,int> t) {
        return std::get<0>(t) + std::get<1>(t);
    }, std::make_tuple(0, 1));

    assert(fibs.take(5) == Collection<int>(std::vector<int> {0,1,1,2,3}));

}
