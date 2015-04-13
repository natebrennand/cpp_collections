#include <vector>
#include <iostream>
#include <cassert>


#include "../collections.h"


int main(){
    auto ints = Collection<int>::range(1,11);

    auto add = [](int x, int y) {return x+y;};
    int i = ints.preduce(add, 3);

    assert(i == 55);
}
