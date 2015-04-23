#ifndef STREAMS_H
#define STREAMS_H

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <pthread.h>
#include <stdio.h>
#include <tuple>
#include <type_traits>
#include <vector>

#include "collections.h"

namespace cpp_collections {

    template<typename T>
    class Stream {
    private:
        T Head;
        std::shared_ptr<Stream<T>> Tail;
        std::function<Stream<T>()> Gen;
    public:

        // default Stream constructor
        Stream<T>(T head, std::function<Stream<T>()> gen) {
            Head = head;
            Tail = nullptr;
            Gen  = gen;
        }

        // Return the first element of the Stream
        T
        head();

        // Create the tail Stream by removing the head
        Stream<T>
        tail() const;

        // Return a Collection with n elements taken from the Stream
        Collection<T>
        take(int n);

        // Return a substream of elements of the Stream that match the
        // predicate function
        Stream<T>
        filter(std::function<bool(T)> func) const;

        // Return the Stream that results from the transformation of each
        // element in the original Stream
        template<typename Function>
        Stream<typename std::result_of<Function(T)>::type>
        map(Function func) const;

    };

    // Return the first element of the Stream
    template<typename T>
    T
    Stream<T>::head() {
        return Head;
    }

    // Create the tail Stream by removing the head
    template<typename T>
    Stream<T>
    Stream<T>::tail() const {
        if (Tail)
            return *Tail;
        return Gen();
    }

    // Return a Collection with n elements taken from the Stream
    template<typename T>
    Collection<T>
    Stream<T>::take(int n) {
        std::vector<T> list(n);
        Stream<T> temp = *this;
        for (int i = 0; i < n; i++) { 
            list[i] = temp.head();
            temp = temp.tail();
        }
        return Collection<T>(list);
    }

    // Return a substream of elements of the Stream that match the
    // predicate function
    template<typename T>
    Stream<T>
    Stream<T>::filter(std::function<bool(T)> func) const {

        Stream<T> temp = tail();
    
        if (func(Head))
            return Stream<T>(Head, [temp, func]() -> Stream<T> {
                return temp.filter(func);
            });

        return temp.filter(func);
    }

    // Return the Stream that results from the transformation of each
    // element in the original Stream
    template<typename T>
    template<typename Function>
    Stream<typename std::result_of<Function(T)>::type>
    Stream<T>::map(Function func) const {
        using return_type = typename std::result_of<Function(T)>::type;

        Stream<T> temp = tail();

        return Stream<return_type>(func(Head), [temp, func]() -> Stream<return_type> {
            return temp.map(func);
        });
    }
    
    // Construct a Stream, starting at n, incrementing by step
    template<typename T>
    Stream<T>
    from(T n, T step) {
        return Stream<T>(n, [=]() -> Stream<T> {
            return from(n + step, step);
        });
    }

    // Construct a Stream, starting at n, incrementing by 1
    template<typename T>
    Stream<T>
    from(T n) {
        return Stream<T>(n, [=]() -> Stream<T> {
            return from(n + T(1), T(1));
        });
    }

    // Return a Stream of tuples, where each tuple contains the elements of 
    // the zipped Streams that occur at the same position
    template<typename ...U>
    Stream<std::tuple<U...>>
    zip(Stream<U>... other_stream) {
        using return_type = std::tuple<U...>;
  
        std::allocator<return_type> alloc;
        return_type *tmp = alloc.allocate(1);
        alloc.construct(tmp, std::move(other_stream.head())...);
        return Stream<return_type>(*tmp, [=]() -> Stream<return_type> {
            return zip(other_stream.tail()...); 
        });
    }

    // Generalizes zip by zipping with the function given as the first argument
    // instead of a tupling function
    template<typename Function, typename ...U>
    Stream<typename std::result_of<Function(U...)>::type>
    zipWith(Function func, Stream<U>... other_stream) {
        using return_type = typename std::result_of<Function(U...)>::type;

        std::allocator<return_type> alloc;
        return_type *tmp = alloc.allocate(1);
        alloc.construct(tmp, func(other_stream.head()...));
        return Stream<return_type>(*tmp, [=]() -> Stream<return_type> {
            return zipWith(func, other_stream.tail()...);                
        });
    }

}

#endif
