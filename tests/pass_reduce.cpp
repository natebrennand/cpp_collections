#include <vector>
#include <iostream>

int main() {

    auto add = [](int x, int y) {return x+y;};
    std::vector<int> ints = {2, 3, 4, 5};
    Collection<int> ints2 = Collection<int>(ints);

    auto i = ints2.reduce(add, 1);
    std::cout << i << std::endl;
}
