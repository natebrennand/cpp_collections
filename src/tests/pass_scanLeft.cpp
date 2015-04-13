#include <vector>
#include <iostream>
#include <cassert>


#include "../collections.h"


int main(){
    auto ints = Collection<int>(std::vector<int> {1, 2, 3, 4});

    auto add = [](int x, int y) {return x+y;};

    auto ints2 = ints.scanLeft(add, 1); 

    assert(ints2 == Collection<int>(std::vector<int> {1, 2, 4, 7, 11}));
}
