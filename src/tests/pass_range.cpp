#include <vector>
#include <cassert>

#include "../collections.h"


int main() {

    auto ints = Collection<int>::range(4);
    assert(ints[0] == 0);
    assert(ints[1] == 1);
    assert(ints.size() == 4);

    auto ints2 = Collection<int>::range(2,4);
    assert(ints2[0] == 2);
    assert(ints2[1] == 3);
    assert(ints2.size() == 2);

    Collection<float> floats = Collection<float>::range(4);
    assert(floats[0] == 0.0);
    assert(floats.size() == 4);

    auto ints3 = Collection<int>::range(4).map([](int i){ return i+1; });
    assert(ints3[0] == 1);
    assert(ints3[1] == 2);
}
