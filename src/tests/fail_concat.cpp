#include <list>
#include <array>
#include <vector>
#include <iostream>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {

    Collection<int> a = range(5);

    Collection<char> b = Collection<char>(std::vector<char> {'a','b','c'});
    
    auto c = concat(a, b);
    
    c.print();
}
