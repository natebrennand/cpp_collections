#include <cmath>
#include <ostream>
#include <vector>
#include <cassert>

#include "../cpp_collections.h"

using namespace cpp_collections;

int main() {

    auto count = iterate(1, [](int x) { return x+1; });

    assert(count.take(5) == Collection<int>(std::vector<int> {1,2,3,4,5}));
}
