
#include <vector>
#include <string>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {

    auto strs = range("hello");
    assert(strs[0] == "0");

}
