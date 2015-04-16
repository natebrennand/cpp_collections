#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main(){
    Collection<int> ints = range(10);

    auto incr = [](int x) {return x+1;};
    auto result = ints.tmap(incr, 3);

    result.print();

    assert(result == range(1, 11));
}
