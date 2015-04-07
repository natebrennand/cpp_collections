
#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <array>
#include <vector>
#include <list>
#include <iterator>
#include <functional>
#include <iostream>
#include <tuple>


template<typename T>
class Collection {
private:
    std::vector<T> Data;
public:

    // std::vector constructor
    Collection<T>(std::vector<T> d) {
        Data = d;
    };

    // std::list constructor
    Collection<T>(std::list<T> d) {
        Data = std::vector<T>(d.size());
        int index = 0;
        for (auto i : d)
            Data[index++] = i;
    };

    // std::array constructor
    template<std::size_t SIZE>
    Collection<T>(std::array<T, SIZE> d) {
        Data = std::vector<T>(SIZE);
        int index = 0;
        for (auto i : d)
            Data[index++] = i;
    };

    // C-style array constructor (requires length)
    Collection<T>(T d[], int len) {
        Data.assign(d, d + len);
    };

    std::vector<T>
    vector();

    std::list<T>
    list();

    T
    head();

    T
    last();

    Collection<T>
    init();

    Collection<T>
    tail();

    void 
    pop_head();

    Collection<T>
    filter(std::function<bool(T)> func);

    Collection<T>
    slice(int low, int high);

    static
    Collection<T>
    range(int size);

    static
    Collection<T>
    range(int low, int high);

    template<typename Function>
    Collection<typename std::result_of<Function(T)>::type>
    map(Function func);

    T
    fold(std::function<T(T, T)> func);

    template<typename ...U>
    static
    Collection<std::tuple<U...>>
    zip(Collection<U>... other_list);


    template<typename Function, typename ...U>
    static
    Collection<typename std::result_of<Function(U...)>::type>
    zipWith(Function func, Collection<U>... other_list);

    template<typename Function, typename U>
    typename std::result_of<Function(U, T)>::type
    reduce(Function func, U init);

    T
    operator[] (const int index);

    bool
    operator==(const Collection<T> other) {
        return Data == other.Data;
    }

    friend std::ostream&
    operator<<(std::ostream& stream, const Collection<T>& f) {
        stream << "[";

        for (int i = 0; i < f.Data.size() - 1; i++)
            stream << f.Data[i] << ",";
        stream << f.Data[f.Data.size() - 1];

        stream << "]";
        return stream;
    }

    int
    size();

    void
    print();

};


template<typename T>
std::vector<T>
Collection<T>::vector() {
    return Data;
};


template<typename T>
std::list<T>
Collection<T>::list() {
    return std::list<T>(std::begin(Data), std::end(Data));
};


template<typename T>
T
Collection<T>::head() {
    // TODO: add emptiness checking
    return Data[0];
};


template<typename T>
T
Collection<T>::last() {
    // TODO: add emptiness checking
    return Data[Data.size() - 1];
};


template<typename T>
Collection<T>
Collection<T>::init() {
    // TODO: add emptiness checking
    return Collection<T>(std::vector<T>(Data.begin(), Data.end() - 1));
};


template<typename T>
Collection<T>
Collection<T>::tail() {
    // TODO: add emptiness checking
    return Collection<T>(std::vector<T>(Data.begin() + 1, Data.end()));
};


template<typename T>
void
Collection<T>::pop_head() {
    // TODO: add emptiness checking
    Data.erase(Data.begin());
};


template<typename T>
Collection<T>
Collection<T>::filter(std::function<bool(T)> func) {
    std::vector<T> list;
    for (auto i : Data)
        if (func(i))
            list.push_back(i);
    return Collection<T>(list);
};


template<typename T>
Collection<T>
Collection<T>::slice(int low, int high) {
    std::vector<T> list(high-low);
    for (int i = 0; i < high-low; i++)
        list[i] = Data[i+low];
    return Collection<T>(list);
};


template<typename T>
Collection<T>
Collection<T>::range(int size) {
    std::vector<T> list(size);
    for (int i = 0; i < size; i++)
        list[i] = T(i);
    return Collection<T>(list);
};


template<typename T>
Collection<T>
Collection<T>::range(int low, int high) {
    std::vector<T> list(high-low);
    for (int i = 0; i < high-low; i++)
        list[i] = T(low + i);
    return Collection<T>(list);
};

template<typename T>
template<typename Function>
Collection<typename std::result_of<Function(T)>::type>
Collection<T>::map(Function func) {
    using return_type = typename std::result_of<Function(T)>::type;

    std::vector<return_type> list(size());
    for (int i = 0; i < Data.size(); i++)
        list.push_back(func(Data[i]));
    return Collection<return_type>(list);
};


template<typename T>
T
Collection<T>::fold(std::function<T(T, T)> func) {
    // TODO: bounds checking
    T val = func(Data[0], Data[1]);
    for (int i = 2; i < Data.size(); i++)
        val = func(val, Data[i]);

    return val;
};


template<typename T>
template<typename ...U>
Collection<std::tuple<U...>>
Collection<T>::zip(Collection<U>... other_list) {
    // TODO: list size checking
    using return_type = std::tuple<U...>;

    int size = std::min({other_list.size()...});
    std::vector<return_type> list;
    for (int i = 0; i < size; i++) {
        list.emplace_back(std::move(other_list.head())...);
        [](...){} ((other_list.pop_head(), 0)...); 
    }
    return Collection<return_type>(list);
};


template<typename T>
template<typename Function, typename ...U>
Collection<typename std::result_of<Function(U...)>::type>
Collection<T>::zipWith(Function func, Collection<U>... other_list) {
    // TODO: list size checking
    // TODO: check that func takes as many arguments as there are lists
    using return_type = typename std::result_of<Function(U...)>::type;

    int size = std::min({other_list.size()...});
    std::vector<return_type> list;
    for (int i = 0; i < size; i++) {
        list.emplace_back(func(other_list.head()...));
        [](...){} ((other_list.pop_head(), 0)...);
    }
    return Collection<return_type>(list);
};


template<typename T>
template<typename Function, typename U>
typename std::result_of<Function(U, T)>::type
Collection<T>::reduce(Function func, U init) {
    using return_type = U;
    /*
    static_assert(
        std::is_same<
            decltype(func),
            std::function<U(U, T)>
        >::value,
           "Reduce fn must return the same type as the initial value");
    */

    // TODO: bounds checking
    return_type val = func(init, Data[0]);
    for (int i = 1; i < Data.size(); i++)
        val = func(val, Data[i]);

    return val;
};


template<typename T>
T
Collection<T>::operator[] (const int index) {
    return Data[index];
};


template<typename T>
void
Collection<T>::print() {
    std::cout << *this << std::endl;
};


template<typename T>
int
Collection<T>::size() {
    return Data.size();
};


#endif

