#include <vector>
#include <cassert>

#include "../streams.h"

using namespace cpp_collections;

int main() {

    auto stream1 = from(1);

    auto stream2 = from(1);

    auto stream3 = from(1);

    auto add = [](int x, int y, int z) { return x + y + z; };

    auto stream4 = zipWith(add, stream1, stream2, stream3);
  
    assert(stream4.take(3) == Collection<int>(std::vector<int> {3, 6, 9}));

}
