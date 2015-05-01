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
    
    Collection<int> c = range(11, 21);

    Collection<int> d = concat(a, b, c);
    
    std::cout << d << std::endl;

    assert(d == range(21));
}
