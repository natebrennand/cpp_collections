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

#include "utility.h"
#include "collections.h"

namespace cpp_collections {

    template<typename T>
    class Stream {
    private:
        T Head;
        std::shared_ptr<Stream<T>> Tail;
        std::function<Stream<T>()> Gen;
    public:

        // Default Stream constructor
        Stream<T>(T head, std::function<Stream<T>()> gen) {
            Head = head;
            Tail = nullptr;
            Gen  = gen;
        }

        // Tail-only Stream constructor
        Stream<T>(T head, Stream<T> tail) {
            Head = head;
            Tail = std::make_shared<Stream<T>>(tail);
            Gen  = nullptr;
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

    //
    // Non-member Functions
    //

    template<typename T>
    Stream<T>
    cons(T value, Stream<T> other) {
        return Stream<T>(value, other);
    }

    template<typename T>
    Stream<T>
    operator&(T value, Stream<T> other) {
        return cons(value, other);
    }

    // Return a substream of elements of the Stream that match the
    // predicate function
    template<typename T>
    Stream<T>
    Stream<T>::filter(std::function<bool(T)> func) const {
        Stream<T> temp = tail();
    
        if (func(Head)) {
            return Stream<T>(Head, [temp, func]() -> Stream<T> {
                return temp.filter(func);
            });
        }
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
    from(T n, T step=1) {
        return Stream<T>(n, [=]() -> Stream<T> {
            return from(n + T(step), T(step));
        });
    }

    // Construct a Stream whose values are the return values from repeated
    // calls the generation function which takes no arguments.
    template<typename Function>
    Stream<typename std::result_of<Function()>::type>
    generate(Function func) {
        using return_type = typename std::result_of<Function()>::type;
        return Stream<return_type>(func(), [=]() -> Stream<return_type> {
            return generate(func);
        });
    }

    // Construct a Stream that consists of a repeated value.
    template<typename T>
    Stream<T>
    repeat(T value) {
        return Stream<T>(value, [=]() -> Stream<T> {
            return repeat(value);
        });
    }

    // Given an initial value x and function f, return the Stream produced by
    // x, f(x), f(f(x)), and so on.
    template<typename T, typename Function>
    Stream<T>
    iterate(T value, Function func) {
        return Stream<T>(value, [=]() -> Stream<T> {
            return iterate(func(value), func);
        });
    }

    // Return the Stream that is the recurrence relation starting with an
    // intial set of values. For example, if we have two initial values a1 and
    // a2, the output of the stream would be: a1, a2, a3=f(a1,a2), a4=f(a2,a3),
    // and so on.
    template<typename Tuple, typename Function>
    Stream<typename std::result_of<Function(Tuple)>::type>
    recurrence(Function func, Tuple t) {
        using return_type = typename std::result_of<Function(Tuple)>::type;

        auto sub_tuple = get_part<1, std::tuple_size<Tuple>::value>(t);
        auto new_tuple = std::tuple_cat(sub_tuple, std::make_tuple(func(t)));
        return Stream<return_type>(std::get<0>(t), [=]() -> Stream<return_type> {
            return recurrence(func, new_tuple);
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
