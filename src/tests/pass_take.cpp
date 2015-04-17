#include <vector>
#include <cassert>

#include "../streams.h"

using namespace cpp_collections;

int main() {

    auto x = from(3);

    auto y = x.take(3);

    assert(y == Collection<int>(std::vector<int> {3,4,5}));

    x = from(3, 2);

    y = x.take(3);

    assert(y == Collection<int>(std::vector<int> {3,5,7}));

}
