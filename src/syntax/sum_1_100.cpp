#include <ostream>
#include <vector>

#include "../collections.h"

int main() {

    // standard syntax
    int sum = 0;
    for (int i = 0; i <= 100; i++)
        sum += i; 

    // Collection syntax
    sum = Collection<int>::range(1,101)
              .fold([](int x, int y) {return x + y;});
}
