#include <vector>
#include <iostream>
#include <cassert>

#include "collections.h"


int main(){
    Collection<int> ints = Collection<int>(std::vector<int> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    //incr = [](int x) {return x+1;};

    //auto result  = ints.conmap(incr, 2);

    ints.bar();

    //result.print();
}
