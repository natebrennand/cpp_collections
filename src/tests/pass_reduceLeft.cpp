#include <vector>
#include <iostream>
#include <cassert>


#include "../collections.h"


int main(){
    auto ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5});

    auto add = [](int x, int y) {return x+y;};
    int i = ints.reduceLeft(add);

    assert(i == 15);
}
