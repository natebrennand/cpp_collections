#include <vector>
#include <cassert>
#include <sstream>

#include "../collections.h"


int main() {

    auto ints = Collection<int>::range(4);

    std::stringstream ss;
    ss << ints;
    assert(ss.str() == "[0,1,2,3]");
}
