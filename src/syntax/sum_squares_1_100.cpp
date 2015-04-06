#include <ostream>
#include <vector>

#include "../collections.h"

int main() {

    // standard syntax
    int sum = 0;
    std::vector<int> squares = std::vector<int>(100);
    for (int i = 0; i < 100; i++)
        squares[i] = (i+1) * (i+1);
    for (int i : squares)
        sum += i;

    // Collection syntax
    sum = Collection<int>::range(1,101)
              .map([](int x) {return x * x;})
              .fold([](int x, int y) {return x + y;});
}