#include <vector>
#include <iostream>


int main(){
    auto add = [](int x, int y) {return x+y;};
    std::vector<int> ints = {1, 2, 3, 4, 5};
    auto i = Collection<int>(ints).fold(add);
    std::cout << i << std::endl;
}
