#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;


int main(){
    Collection<int> ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5});

    auto incr = [](int x) {return x+1;};
    auto result  = ints.map(incr);

    assert(result == Collection<int>(std::vector<int> {2, 3, 4, 5, 6}));
}
