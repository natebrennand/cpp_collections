#include <vector>
#include <cassert>

#include "../cpp_collections.h"

using namespace cpp_collections;

int main() {

    auto stream1 = repeat(1);
    auto stream2 = repeat('a');

    assert(stream1.take(3) == Collection<int>(std::vector<int> {1,1,1}));

    assert(stream2.take(3) == Collection<char>(std::vector<char> {'a','a','a'}));
}
