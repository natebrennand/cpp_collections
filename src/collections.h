#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <stdio.h>
#include <thread>
#include <tuple>
#include <type_traits>
#include <vector>

int detectedThreads = (std::thread::hardware_concurrency() == 0) ? \
    std::thread::hardware_concurrency() : 4;

namespace cpp_collections {


    template<typename T>
    class Collection {
    private:
        std::vector<T> Data;
    public:

        // std::vector constructor
        Collection<T>(const std::vector<T>& d) {
            Data = d;
        };

        // construct an empty collection of size 'size'
        Collection<T>(int size) {
            Data = std::vector<T>(size);
        };

        // construct an empty Collection
        Collection<T>() {
            Data = std::vector<T>();
        };

        // std::list constructor
        Collection<T>(const std::list<T>& d) {
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

        // Overload the [] operator
        T
        operator[] (const int index) {
            return Data[index];
        };

        // Overload the == operator
        bool
        operator==(const Collection<T>& other) {
            return Data == other.Data;
        };

        // Overload the << operator
        friend std::ostream&
        operator<<(std::ostream& stream, const Collection<T>& f) {
            stream << "[";

            for (int i = 0; i < f.Data.size() - 1; i++)
                stream << f.Data[i] << ",";
            stream << f.Data[f.Data.size() - 1];

            stream << "]";
            return stream;
        };

        // Return Collection as a std::vector
        std::vector<T>
        vector() {
            return Data;
        };

        // Return Collection as a std::list
        std::list<T>
        list() {
            return std::list<T>(std::begin(Data), std::end(Data));
        };

        // Return the size of the Collection
        int
        size();

        // Return the first element
        T
        head();

        // Return the last element
        T
        last();

        // Return all the elements except the last
        Collection<T>
        init();

        // Return all the elements except the head
        Collection<T>
        tail();

        // Apply a function to all the elements in the Collection
        void
        each(std::function<void(T)> func);

        // Return the elements that pass a predicate function
        Collection<T>
        filter(std::function<bool(T)> func);

        // Return the elements whose indices are within the range [low, high)
        Collection<T>
        slice(int low, int high);

        // Return the Collection that results from the transformation of each
        // element in the original Collection
        template<typename Function>
        Collection<typename std::result_of<Function(T)>::type>
        map(Function func) const;

        // An alternative implementation of map that uses multiple concurrent
        // std::threads to speed up processing
        template<typename Function>
        Collection<typename std::result_of<Function(T)>::type>
        tmap(Function func, int threads=detectedThreads) const;

        // Return the result of the application of the same binary operator on
        // adjacent pairs of elements in the Collection, starting from the left
        T
        reduceLeft(std::function<T(T, T)> func);

        // Return the result of the application of the same binary operator on
        // adjacent pairs of elements in the Collection, starting from the right
        T
        reduceRight(std::function<T(T, T)> func);

        // An alternative implementation of reduce that uses multiple concurrent
        // std::threads to speed up processing (note that the function passed to
        // treduce must be commutative to achieve accurate result)
        T
        treduce(std::function<T(T, T)> func, int threads=detectedThreads);

        // Return the result of the application of the same binary operator on
        // all elements in the Collection as well as an initial value, starting
        // from the left
        template<typename Function, typename U>
        typename std::result_of<Function(U, T)>::type
        foldLeft(Function func, U init);

        // Return the result of the application of the same binary operator on
        // all elements in the Collection as well as an initial value, starting
        // from the right
        template<typename Function, typename U>
        typename std::result_of<Function(U, T)>::type
        foldRight(Function func, U init);

        // Returns the intermediate results of the binary accumulation of the
        // elements in a Collection as well as an initial value, starting from the
        // left
        template<typename Function, typename U>
        Collection<typename std::result_of<Function(U, T)>::type>
        scanLeft(Function func, U init);

        // Returns the intermediate results of the binary accumulation of the
        // elements in a Collection as well as an initial value, starting from the
        // right
        template<typename Function, typename U>
        Collection<typename std::result_of<Function(U, T)>::type>
        scanRight(Function func, U init);

    };

    // --------------------------
    // BASIC OPERATIONS
    // --------------------------

    // Return the size of the Collection
    template<typename T>
    int
    Collection<T>::size() {
        return Data.size();
    }

    // Return the first element
    template<typename T>
    T
    Collection<T>::head() {
        // TODO: add emptiness checking
        return Data[0];
    }

    // Return the last element
    template<typename T>
    T
    Collection<T>::last() {
        // TODO: add emptiness checking
        return Data[Data.size() - 1];
    }

    // Return all the elements except the last
    template<typename T>
    Collection<T>
    Collection<T>::init() {
        // TODO: add emptiness checking
        return Collection<T>(std::vector<T>(Data.begin(), Data.end() - 1));
    }

    // Return all the elements except the head
    template<typename T>
    Collection<T>
    Collection<T>::tail() {
        // TODO: add emptiness checking
        return Collection<T>(std::vector<T>(Data.begin() + 1, Data.end()));
    }

    // --------------------------
    // ADVANCED OPERATIONS
    // --------------------------

    // Apply a function to all the elements in the Collection
    template<typename T>
    void
    Collection<T>::each(std::function<void(T)> func) {
        for (auto i : Data)
            func(i);
    }

    // Return the elements that pass a predicate function
    template<typename T>
    Collection<T>
    Collection<T>::filter(std::function<bool(T)> func) {
        std::vector<T> list;
        for (auto i : Data)
            if (func(i))
                list.push_back(i);
        return Collection<T>(list);
    }

    // Return the elements whose indices are within the range [low, high)
    template<typename T>
    Collection<T>
    Collection<T>::slice(int low, int high) {
        std::vector<T> list(high-low);
        for (int i = 0; i < high-low; i++)
            list[i] = Data[i+low];
        return Collection<T>(list);
    }

    // Return the Collection that results from the transformation of each
    // element in the original Collection
    template<typename T>
    template<typename Function>
    Collection<typename std::result_of<Function(T)>::type>
    Collection<T>::map(Function func) const {
        using return_type = typename std::result_of<Function(T)>::type;

        std::vector<return_type> list(Data.size());
        for (int i = 0; i < Data.size(); i++)
            list[i] = func(Data[i]);
        return Collection<return_type>(list);
    }

    template<typename T, typename Function>
    void
    tmap_thread(int begin, int end, Function func, std::vector<T>& list) {
        for (int i = begin; i < end && i < list.size(); i++)
            list[i] = func(list[i]);
    }

    // An alternative implementation of map that uses multiple concurrent
    // std::threads to speed up processing
    template<typename T>
    template<typename Function>
    Collection<typename std::result_of<Function(T)>::type>
    Collection<T>::tmap(Function func, int threads) const {
        std::vector<std::thread> thread_pool(threads);

        // TODO: add bounds checking
        int chunk = Data.size() / threads;
        int extra = Data.size() - chunk*threads;
        std::vector<int> indices(extra, chunk+1);
        std::vector<int> normal(threads-extra, chunk);
        indices.insert(indices.end(), normal.begin(), normal.end());

        std::vector<T> NewData = Data;

        int start = 0;
        for (int i = 0; i < threads; i++) {
            int end = start + indices[i];

            thread_pool[i] = std::thread ([=, &NewData]() {
                tmap_thread(start, end, func, NewData);
            });

            start = end;
        }

        for (int i = 0; i < threads; i++)
            thread_pool[i].join();

        std::cout << NewData[0] << std::endl;
        return Collection<T>(NewData);
    }

    // Return the result of the application of the same binary operator on
    // adjacent pairs of elements in the Collection, starting from the left
    template<typename T>
    T
    Collection<T>::reduceLeft(std::function<T(T, T)> func) {
        // TODO: bounds checking
        T val = func(Data[0], Data[1]);
        for (int i = 2; i < Data.size(); i++)
            val = func(val, Data[i]);

        return val;
    }

    // Return the result of the application of the same binary operator on
    // adjacent pairs of elements in the Collection, starting from the right
    template<typename T>
    T
    Collection<T>::reduceRight(std::function<T(T, T)> func) {
        // TODO: bounds checking
        T val = func(Data[Data.size() - 1], Data[Data.size() - 2]);
        for (int i = Data.size() - 3; i >= 0; i--)
            val = func(val, Data[i]);

        return val;
    }

    template<typename T>
    void
    treduce_thread(int tid, int begin, int end, std::function<T(T, T)> func,
                   std::vector<T>& list, std::vector<T>& results) {
        T val = func(list[begin], list[begin + 1]);
        for (int i = begin + 2; i < end && i < list.size(); i++)
            val = func(val, list[i]);
        results[tid] = val;
    }

    // An alternative implementation of reduce that uses multiple concurrent
    // threads to speed up processing (note that the function passed to
    // treduce must be commutative to achieve accurate result)
    template<typename T>
    T
    Collection<T>::treduce(std::function<T(T, T)> func, int threads) {
        std::vector<std::thread> thread_pool(threads);
        std::vector<T> results(threads);

        // TODO: add bounds checking
        int chunk = Data.size() / threads;
        int extra = Data.size() - chunk*threads;
        std::vector<int> indices(extra, chunk+1);
        std::vector<int> normal(threads-extra, chunk);
        indices.insert(indices.end(), normal.begin(), normal.end());

        int start = 0;
        for (int i = 0; i < threads; i++) {
            int end = start + indices[i];

            thread_pool[i] = std::thread ([=, &results]() {
                treduce_thread(i, start, end, func, Data, results);
            });

            start = end;
        }

        for (int i = 0; i < thread_pool.size(); i++)
            thread_pool[i].join();

        // TODO: check that results has size greater than 1
        T val = func(results[0], results[1]);
        for (int i = 2; i < results.size(); i++)
            val = func(val, results[i]);
        return val;
    }

    // Return the result of the application of the same binary operator on
    // all elements in the Collection as well as an initial value, starting
    // from the left
    template<typename T>
    template<typename Function, typename U>
    typename std::result_of<Function(U, T)>::type
    Collection<T>::foldLeft(Function func, U init) {
        using return_type = typename std::result_of<Function(U, T)>::type;
        static_assert(std::is_same<return_type, U>::value,
            "Fold fn must return the same type as the initial value");

        // TODO: bounds checking
        return_type val = func(init, Data[0]);
        for (int i = 1; i < Data.size(); i++)
            val = func(val, Data[i]);

        return val;
    }

    // Return the result of the application of the same binary operator on
    // all elements in the Collection as well as an initial value, starting
    // from the right
    template<typename T>
    template<typename Function, typename U>
    typename std::result_of<Function(U, T)>::type
    Collection<T>::foldRight(Function func, U init) {
        using return_type = typename std::result_of<Function(U, T)>::type;
        static_assert(std::is_same<return_type, U>::value,
            "Fold fn must return the same type as the initial value");

        // TODO: bounds checking
        return_type val = func(init, Data[Data.size() - 1]);
        for (int i = Data.size() - 2; i >= 0; i--)
            val = func(val, Data[i]);

        return val;
    }

    // Returns the intermediate results of the binary accumulation of the
    // elements in a Collection as well as an initial value, starting from the
    // left
    template<typename T>
    template<typename Function, typename U>
    Collection<typename std::result_of<Function(U, T)>::type>
    Collection<T>::scanLeft(Function func, U init) {
        using return_type = typename std::result_of<Function(U, T)>::type;
        static_assert(std::is_same<return_type, U>::value,
            "Scan fn must return the same type as the initial value");

        std::vector<return_type> list(Data.size() + 1);
        list[0] = init;
        for (int i = 0; i < Data.size(); i++)
            list[i + 1] = func(list[i], Data[i]);
        return Collection<return_type>(list);
    }

    // Returns the intermediate results of the binary accumulation of the
    // elements in a Collection as well as an initial value, starting from the
    // right
    template<typename T>
    template<typename Function, typename U>
    Collection<typename std::result_of<Function(U, T)>::type>
    Collection<T>::scanRight(Function func, U init) {
        using return_type = typename std::result_of<Function(U, T)>::type;
        static_assert(std::is_same<return_type, U>::value,
            "Scan fn must return the same type as the initial value");

        std::vector<return_type> list(Data.size() + 1);
        list[list.size() - 1] = init;
        for (int i = list.size() - 2; i >= 0; i--)
            list[i] = func(list[i + 1], Data[i]);
        return Collection<return_type>(list);
    }

    // --------------------------
    // NON-MEMBER FUNCTIONS
    // --------------------------

    template<typename T>
    void
    concat_helper(std::vector<T>& list, Collection<T>& other_list, int& index) {
        for (int i = 0; i < other_list.size(); i++)
            list[index++] = other_list[i];
    }

    // Concatenate an arbitrary number of Collections
    template<typename T, typename ...Collections>
    Collection<T>
    concat(Collection<T>& original, Collections... other_list) {
        // TODO: Check that all arguments are Collections of the same type with
        // a static_assert. However, note that this check is already being made
        // implicitly by concat_helper 
        int size = original.size();
        int get_size[]{0, (size += other_list.size(), 0)...};
        std::vector<T> list(size);

        int index = 0;
        concat_helper(list, original, index);
        int concatenate[]{0, (concat_helper(list, other_list, index), 0)...};
        return Collection<T>(list);
    }

    // Return Collection of numeric types over the range [0, size)
    template<typename T>
    const Collection<T>
    range(T size) {
        static_assert(std::is_arithmetic<T>::value,
            "You must pass range arithmetic type parameters");

        std::vector<T> v(size);
        for (int i = 0; i < size; i++)
            v[i] = T(i);

        const auto tmp = Collection<T>(v);
        return tmp;
    }

    // Return Collection of numeric types over the range [low, high)
    template<typename T>
    const Collection<T>
    range(T low, T high) {
        static_assert(std::is_arithmetic<T>::value,
            "You must pass range arithmetic type parameters");

        std::vector<T> v(high-low);
        for (int i = 0; i < high-low; i++)
            v[i] = T(low + i);

        const auto tmp = Collection<T>(v);
        return tmp;
    }


    // Return a Collection of tuples, where each tuple contains the elements of
    // the zipped lists that occur at the same position
    template<typename ...U>
    Collection<std::tuple<U...>>
    zip(Collection<U>&... other_list) {
        // TODO: list size checking
        using return_type = std::tuple<U...>;

        std::vector<return_type> list(std::min({other_list.size()...}));
        for (int i = 0; i < list.size(); i++) {
            list[i] = std::make_tuple(other_list[i]...);
        }
        return Collection<return_type>(list);
    }

    // Generalizes zip by zipping with the function given as the first argument
    // instead of a tupling function
    template<typename Function, typename ...U>
    Collection<typename std::result_of<Function(U...)>::type>
    zipWith(Function func, Collection<U>&... other_list) {
        // TODO: list size checking
        // TODO: check that func takes as many arguments as there are lists
        using return_type = typename std::result_of<Function(U...)>::type;

        std::vector<return_type> list(std::min({other_list.size()...}));
        for (int i = 0; i < list.size(); i++) {
            list[i] = func(other_list[i]...);
        }
        return Collection<return_type>(list);
    }
}

#endif
