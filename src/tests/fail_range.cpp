
#include <vector>
#include <string>
#include <cassert>

#include "../collections.h"


int main() {

    auto strs = Collection<std::string>::range(4);
    assert(strs[0] == "0");

}
