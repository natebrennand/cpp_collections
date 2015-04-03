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

    template<typename F>
    Collection<F> map(std::function<F(T)> func) {
        std::vector<F> list;
        for (int i = 0; i < Data.size(); i++)
            list.push_back(func(Data[i]));
        return Collection<F>(list);
    };

    /*
    template<typename F (typename T)>
    Collection<F> map(std::function<F(T)> func) {
        std::vector<F> list;
        for (int i = 0; i < Data.size(); i++)
            list.push_back(func(Data[i]));
        return Collection<F>(list);
    };
    */

    //template<typename F>
    //Collection<F> map(std::function<F(T)> func) {
    //    std::vector<F> list;
    //    for (int i = 0; i < Data.size(); i++)
    //        list.push_back(func(Data[i]));
    //    return Collection<F>(list);
    //};


    //template<typename X func(typename T)>
    // Collection<X> map(std::)
};


int increment(int i){
    return i + 1;
}


int main() {
    std::vector<int> v = {'a', 'b', 'c', 'd'};
    Collection<int> c = Collection<int>(v);

    // auto inc = [](int x) {return x+1;};
    Collection<int> d = c.map<int>(increment);
    for (int i: d.Data)
        std::cout << i;


    auto ascii_from_val = [](int x) {return (char)x;};
    Collection<char> e = c.map<char>(ascii_from_val);
    for (auto i: e.Data)
        std::cout << i << std::endl;
    std::cout << std::endl;


    /*
    std::vector<char> chars = {'a', 'b', 'c', 'd'};
    for (int i: chars)
        std::cout << i;
    std::cout << std::endl;
    */
    Collection<char> z = c.map(ascii_from_val);

}

