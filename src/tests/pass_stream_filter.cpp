#include <vector>
#include <cassert>

#include "../streams.h"

using namespace cpp_collections;

int main() {

    auto evens = from(3).filter([](int x) { return x % 2 == 0; }).take(3);
  
    assert(evens == Collection<int>(std::vector<int> {4,6,8}));

}
