#include <vector>
#include <tuple>
#include <cassert>

#include "../collections.h"

using namespace cpp_collections;

int main() {
    auto ints = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints1 = Collection<int>(std::vector<int> {1, 2, 3});
    auto ints2 = Collection<int>(std::vector<int> {1, 2, 3, 4});

    auto ints3 = zip(ints, ints1, ints2);

    assert(ints3.size() == 3);
    assert(ints3[0] == std::make_tuple(ints[0], ints1[0], ints2[0]));
    assert(ints3[1] == std::make_tuple(ints[1], ints1[1], ints2[1]));
    assert(ints3[2] == std::make_tuple(ints[2], ints1[2], ints2[2]));


    // check that the shorter list size is used
    //auto ints3 = Collection<int>(std::vector<int> {1, 2});
    //auto ints4 = Collection<int>(std::vector<int> {1, 2, 3});
    //auto ints34 = zip(ints3, ints4);
    //assert(ints34[0] == std::make_tuple(1,1));
    //assert(ints34[1] == std::make_tuple(2,2));
    //assert(ints34.size() == 2);
}
