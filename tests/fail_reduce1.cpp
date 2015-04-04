#include <vector>
#include <iostream>




int main() {
    std::vector<int> ints = {2, 3, 4, 5};
    Collection<int> ints2 = Collection<int>(ints);

    auto i = ints2.reduce(add, 1);
    std::cout << i << std::endl;


    auto wrong = [](bool x, int y) {return x+y;};

    auto b = Collection<int>(ints).reduce(wrong, 1);
    std::cout << b << std::endl;
}

