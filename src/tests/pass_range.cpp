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

}
