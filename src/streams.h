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

        Stream<T>(T head, std::function<Stream<T>()> gen) {
            Head = head;
            Tail = nullptr;
            Gen  = gen;
        }

        // Return a collection with n elements taken from the Stream
        Collection<T>
        take(int n);

        // Return the first element of the Stream
        T
        head();

        // Create the tail Stream by removing the head
        Stream<T>
        tail() const;

        // Return a substream of elements of the Stream that match the
        // predicate function
        Stream<T>
        filter(std::function<bool(T)> func) const;

        template<typename Function>
        Stream<typename std::result_of<Function(T)>::type>
        map(Function func) const;
    };

    template<typename T>
    T
    Stream<T>::head() {
        return Head;
    }

    template<typename T>
    Stream<T>
    Stream<T>::tail() const {
        if (Tail)
            return *Tail;
        return Gen();
    }

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
    
    template<typename T>
    Stream<T>
    from(T n, T step) {
        return Stream<T>(n, [=]() -> Stream<T> {
            return from(n + step, step);
        });
    }

    template<typename T>
    Stream<T>
    from(T n) {
        return Stream<T>(n, [=]() -> Stream<T> {
            return from(n + T(1), T(1));
        });
    }

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

}

#endif
