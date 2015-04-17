#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <pthread.h>
#include <stdio.h>
#include <thread>
#include <tuple>
#include <type_traits>
#include <vector>

namespace cpp_collections {

    template<typename T>
    class Collection {
    private:
        std::vector<T> Data;
    public:
    
        // std::vector constructor
        Collection<T>(std::vector<T> d) {
            Data = d;
        };

        Collection<T>(int size) {
            Data = std::vector<T>(size);
        };

        Collection<T>() {
            Data = std::vector<T>();
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
    
        // Print the Collection
        void
        print();
    
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
    
        // Remove the head 
        void
        pop_head();

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
        map(Function func);
    
        // An alternative implementation of map that uses multiple concurrent 
        // pthreads to speed up processing
        template<typename Function>
        Collection<typename std::result_of<Function(T)>::type>
        pmap(Function func, int threads);

        // An alternative implementation of map that uses multiple concurrent
        // std::threads to speed up processing
        template<typename Function>
        Collection<typename std::result_of<Function(T)>::type>
        tmap(Function func, int threads);
    
        // Return the result of the application of the same binary operator on
        // adjacent pairs of elements in the Collection, starting from the left
        T
        reduceLeft(std::function<T(T, T)> func);
    
        // Return the result of the application of the same binary operator on
        // adjacent pairs of elements in the Collection, starting from the right
        T
        reduceRight(std::function<T(T, T)> func);
    
        // An alternative implementation of reduce that uses multiple concurrent
        // pthreads to speed up processing (note that the function passed to
        // preduce must be commutative to achieve accurate result)
        T
        preduce(std::function<T(T, T)> func, int threads);

        // An alternative implementation of reduce that uses multiple concurrent
        // std::threads to speed up processing (note that the function passed to
        // treduce must be commutative to achieve accurate result)
        T
        treduce(std::function<T(T, T)> func, int threads);
       
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

    // Print the Collection
    template<typename T>
    void
    Collection<T>::print() {
        std::cout << *this << std::endl;
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

    // Remove the head 
    template<typename T>
    void
    Collection<T>::pop_head() {
        // TODO: add emptiness checking
        Data.erase(Data.begin());
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
    Collection<T>::map(Function func) {
        using return_type = typename std::result_of<Function(T)>::type;
    
        std::vector<return_type> list(Data.size());
        for (int i = 0; i < Data.size(); i++)
            list[i] = func(Data[i]);
        return Collection<return_type>(list);
    }
    
    template<typename T, typename Function>
    void *
    pmap_thread(void *arg) {
        struct thread_data {
            std::vector<T> *list;
            Function func;
            int begin;
            int end;
        };
        thread_data *data = ((struct thread_data *)arg);
        for (int i = data->begin; i < data->end && i < (*(data->list)).size(); i++)
            (*(data->list))[i] = (data->func)((*(data->list))[i]);
        pthread_exit(NULL);
    }
    
    // An alternative implementation of map that uses multiple concurrent 
    // threads to speed up processing
    template<typename T>
    template<typename Function>
    Collection<typename std::result_of<Function(T)>::type>
    Collection<T>::pmap(Function func, int threads) {
        struct thread_data {
            std::vector<T> *list;
            Function func;
            int begin;
            int end;
        };
        std::vector<pthread_t> thread_pool(threads);
        std::vector<thread_data> thread_data_pool;
    
        // TODO: add bounds checking
        int chunk = Data.size() / threads;
        int extra = Data.size() - chunk*threads;
        std::vector<int> indices(extra, chunk+1);
        std::vector<int> normal(threads-extra, chunk);
        indices.insert(indices.end(), normal.begin(), normal.end());
    
        int start = 0;
        for (int i = 0; i < threads; i++) {
            pthread_t pid;
            thread_pool[i] = pid;
    
            int end = start + indices[i];
            thread_data td = { &Data, func, start, end};
            thread_data_pool.push_back(td);
            start = end;
        }
    
        for (int i = 0; i < threads; i++)
            pthread_create(&(thread_pool[i]), NULL, pmap_thread<T, Function>, &(thread_data_pool[i]));
    
        for (pthread_t i : thread_pool)
            pthread_join(i, NULL);
    
        return Collection<T>(Data);
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
    Collection<T>::tmap(Function func, int threads) {
        std::vector<std::thread> thread_pool(threads);
    
        // TODO: add bounds checking
        int chunk = Data.size() / threads;
        int extra = Data.size() - chunk*threads;
        std::vector<int> indices(extra, chunk+1);
        std::vector<int> normal(threads-extra, chunk);
        indices.insert(indices.end(), normal.begin(), normal.end());
    
        int start = 0;
        for (int i = 0; i < threads; i++) {
            int end = start + indices[i];

            thread_pool[i] = std::thread ([=]() {
                tmap_thread(start, end, func, Data);
            });

            start = end;
        }
    
        for (int i = 0; i < thread_pool.size(); i++)
            thread_pool[i].join();
    
        return Collection<T>(Data);
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
    void *
    preduce_thread(void *arg) {
        struct thread_data {
            std::vector<T> *list;
            std::function<T(T, T)> func;
            int begin;
            int end;
            T retval;
        };
        thread_data *data = ((struct thread_data *)arg);
        std::vector<T> list = *(data->list);
    
        T val = (data->func)(list[data->begin], list[data->begin + 1]);
        for (int i = data->begin + 2; i < data->end && i < list.size(); i++)
            val = (data->func)(val, list[i]);
        data->retval = val;
        pthread_exit(NULL);
    }
    
    // An alternative implementation of reduce that uses multiple concurrent
    // threads to speed up processing (note that the function passed to
    // preduce must be commutative to achieve accurate result)
    template<typename T>
    T
    Collection<T>::preduce(std::function<T(T, T)> func, int threads) {
        struct thread_data {
            std::vector<T> *list;
            std::function<T(T, T)> func;
            int begin;
            int end;
            T retval;
        };
        std::vector<pthread_t> thread_pool(threads);
        std::vector<thread_data> thread_data_pool;
    
        int chunk = Data.size() / threads;
        int extra = Data.size() - chunk*threads;
        std::vector<int> indices(extra, chunk+1);
        std::vector<int> normal(threads-extra, chunk);
        indices.insert(indices.end(), normal.begin(), normal.end());
    
        int start = 0;
        for (int i = 0; i < threads; i++) {
            pthread_t pid;
            thread_pool[i] = pid;
    
            int end = start + indices[i];
            thread_data td = { &Data, func, start, end};
            thread_data_pool.push_back(td);
            start = end;
        }
    
        for (int i = 0; i < threads; i++)
            pthread_create(&(thread_pool[i]), NULL, preduce_thread<T>, &(thread_data_pool[i]));
    
        for (pthread_t i : thread_pool)
            pthread_join(i, NULL);
    
        // TODO: add bounds checking
        T val = func(thread_data_pool[0].retval, thread_data_pool[1].retval);
        for (int i = 2; i < threads; i++)
            val = func(val, thread_data_pool[i].retval);
    
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
    Collection<T>
    range(T size) {
        static_assert(std::is_arithmetic<T>::value, 
            "You must pass range arithmetic type parameters");

        std::vector<T> list(size);
        for (int i = 0; i < size; i++)
            list[i] = T(i);
        return Collection<T>(list);
    }

    // Return Collection of numeric types over the range [low, high)
    template<typename T>
    Collection<T>
    range(T low, T high) {
        static_assert(std::is_arithmetic<T>::value, 
            "You must pass range arithmetic type parameters");
        
        std::vector<T> list(high-low);
        for (int i = 0; i < high-low; i++)
            list[i] = T(low + i);
        return Collection<T>(list);
    }

    // Return a Collection of tuples, where each tuple contains the elements of 
    // the zipped lists that occur at the same position
    template<typename ...U>
    Collection<std::tuple<U...>>
    zip(Collection<U>... other_list) {
        // TODO: list size checking
        using return_type = std::tuple<U...>;
    
        int size = std::min(other_list.size()...);
        std::vector<return_type> list(size);
        std::allocator<return_type> alloc;
        for (int i = 0; i < size; i++) {
            return_type *tmp = alloc.allocate(1);
            alloc.construct(tmp, std::move(other_list.head())...);
            list[i] = *tmp;
            [](...){} ((other_list.pop_head(), 0)...);
        }
        return Collection<return_type>(list);
    }
    
    
    // Generalizes zip by zipping with the function given as the first argument
    // instead of a tupling function
    template<typename Function, typename ...U>
    Collection<typename std::result_of<Function(U...)>::type>
    zipWith(Function func, Collection<U>... other_list) {
        // TODO: list size checking
        // TODO: check that func takes as many arguments as there are lists
        using return_type = typename std::result_of<Function(U...)>::type;
    
        int size = std::min(other_list.size()...);
        std::vector<return_type> list(size);
        std::allocator<return_type> alloc;
        for (int i = 0; i < size; i++) {
            return_type *tmp = alloc.allocate(1);
            alloc.construct(tmp, func(other_list.head()...));
            list[i] = *tmp;
            [](...){} ((other_list.pop_head(), 0)...);
        }
        return Collection<return_type>(list);
    }

}

#endif

