#include <vector>
#include <iostream>
#include <cassert>


#include "../collections.h"


int main(){
    auto ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    auto add = [](int x, int y) {return x+y;};
    int i = ints.preduce(add, 3);

    assert(i == 55);
}
