#include <vector>
#include <iostream>
#include <cassert>


#include "../collections.h"


int main(){
    std::vector<int> ints = {1, 2, 3, 4, 5};

    auto add = [](int x, int y) {return x+y;};
    int i = Collection<int>(ints).fold(add);

    assert(i == 15);
    assert(false);
}
