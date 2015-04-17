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

        // Creates Stream: {n, n+1, n+2,...}
        //Stream<T> 
        //from(T n);

        // Creates Stream: {n, n+step, n+(2*step),...} 
        //Stream<T> 
        //from(T n, T step);

        // Return a collection with n elements taken from the Stream
        Collection<T>
        take(int n);

        // Return the first element of the Stream
        T
        head();

        // Create the tail Stream by removing the head
        Stream<T>
        tail();

    };

    template<typename T>
    T
    Stream<T>::head() {
        return Head;
    }

    template<typename T>
    Stream<T>
    Stream<T>::tail() {
        if (Tail)
            return *Tail;
        return Gen();
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
