#include <vector>
#include <cassert>
#include <sstream>

#include "../collections.h"

using namespace cpp_collections;

int main() {

    auto ints = range(4);

    std::stringstream ss;
    ss << ints;
    assert(ss.str() == "[0,1,2,3]");
}
