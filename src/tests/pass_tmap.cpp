#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main(){
    auto incr = [](int x) {return x+1;};

    auto result = range(10).tmap(incr, 3);
    std::cout << result << std::endl;
    assert(result == range(1, 11));

    auto result2 = range(10).tmap(incr, 3);
    std::cout << result << std::endl;
    assert(result2 == range(1, 11));
}
