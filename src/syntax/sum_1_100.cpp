#include <ostream>
#include <vector>

#include "../collections.h"

using namespace cpp_collections;

int main() {

    // standard syntax
    int sum = 0;
    for (int i = 0; i <= 100; i++)
        sum += i; 

    // Collection syntax
    sum = range(1,101).reduceLeft([](int x, int y) {return x + y;});
}
