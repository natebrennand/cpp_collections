#include <vector>
#include <tuple>
#include <cassert>

#include "../streams.h"

using namespace cpp_collections;

int main() {
    auto ints = from(1);
    auto ints1 = from(1);

    auto intsZipped = zip(ints, ints1);

    auto ints2 = intsZipped.take(3);

    assert(ints2[0] == std::make_tuple(1, 1));
    assert(ints2[1] == std::make_tuple(2, 2));
    assert(ints2[2] == std::make_tuple(3, 3));

}
