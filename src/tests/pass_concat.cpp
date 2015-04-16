#include <list>
#include <array>
#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {

    Collection<int> a = range(5);

    Collection<int> b = range(5,11);

    assert(a.concat(b) == range(11));
}
