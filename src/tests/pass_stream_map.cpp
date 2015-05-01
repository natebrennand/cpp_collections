#include <vector>
#include <cassert>

#include "../streams.h"

using namespace cpp_collections;

int main() {

    auto stream1 = from(3).map([](int x) { return x + 1; });

    assert(stream1.take(3) == Collection<int>(std::vector<int> {4,5,6}));

    auto stream2 = from(3).map([](int x) { return 'a'; });

    assert(stream2.take(3) == Collection<char>(std::vector<char> {'a','a','a'}));

}
