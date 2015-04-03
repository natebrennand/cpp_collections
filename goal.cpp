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

    // template<typename F>
    // Collection<F> map(F<T> func);

    // template<typename F>
    // Collection<F> map(std::function<F(T)>);
    // Collection map(F);

    Collection<T> map(std::function<T(T)> func){
        std::vector<T> list;
        for (int i = 0; i < Data.size(); i++)
            list.push_back(func(Data[i]));
        return Collection<T>(list);
    };
};


int main() {
    std::vector<int> v = {1, 2, 3, 4};
    Collection<int> c = Collection<int>(v);

    auto inc = [](int x) {return x+1;};
    Collection<int> d = c.map(inc);
    for (int i: d.Data)
        std::cout << i;
}

