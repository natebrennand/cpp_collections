#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;


int main(){
    auto ints = range(1,11);

    auto add = [](int x, int y) {return x+y;};
    int i = ints.preduce(add, 3);

    assert(i == 55);
}
