#include <vector>
#include <cassert>

#include "../cpp_collections.h"

using namespace cpp_collections;

int main() {

    auto one_gen = []() { return 1; };
    auto a_gen = []() { return 'a'; };

    auto stream1 = generate(one_gen);
    auto stream2 = generate(a_gen);

    assert(stream1.take(3) == Collection<int>(std::vector<int> {1,1,1}));

    assert(stream2.take(3) == Collection<char>(std::vector<char> {'a','a','a'}));
}
