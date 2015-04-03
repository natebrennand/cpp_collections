#include <vector>
#include <functional>
#include <iostream>

template<typename T>
class Collection {
private:
public:
    std::vector<T> Data;
    Collection<T>(std::vector<T> d){
        Data = d;
    };

    template<typename Function>
    Collection<typename std::result_of<Function(T)>::type>
    map(Function func) {
        using return_type = typename std::result_of<Function(T)>::type;

        std::vector<return_type> list;
        for (int i = 0; i < Data.size(); i++)
            list.push_back(func(Data[i]));
        return Collection<return_type>(list);
    };

    template<typename Function>
    T
    fold(Function func) {
        // TODO: bounds checking
        T val = func(Data[0], Data[1]);
        for (int i = 2; i < Data.size(); i++)
            val = func(val, Data[i]);

        return val;
    };
};

int main() {
    std::vector<int> v = {'a', 'b', 'c', 'd'};
    // std::vector<int> v = {1, 2, 3, 4, 5};
    Collection<int> c = Collection<int>(v);

    std::function<int(int)> inc = [](int x) {return x+1;};

    // std::cout << std::result_of<inc> << std::endl;


    // Collection<int> d = c.map(inc);
    // for (int i: d.Data)
    //    std::cout << i;

    auto ascii_from_val = [](int x) {return (char)x;};
    Collection<char> e = c.map(ascii_from_val);
    for (auto i: e.Data)
        std::cout << i << std::endl;
    std::cout << std::endl;


    auto add = [](int x, int y) {return x+y;};
    std::vector<int> ints = {1, 2, 3, 4, 5};
    auto i = Collection<int>(ints).fold(add);
    std::cout << i << std::endl;
}

