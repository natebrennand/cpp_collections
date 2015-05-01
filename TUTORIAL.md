
# Tutorial

Index:

- [Getting Started](#getting-started)
- [Creating Collections](#creating-collections)
- [Non-Member Functions for Collections](#non-member-functions-for-collections)
- [Simple Member Functions for Collections](#simple-member-functions-for-collections)
  - [Returning Collections](#returning-collections)
  - [List Processing](#list-processing)
- [Advanced Member Functions for Collections](#advanced-member-functions-for-collections)
  - [Mapping functions](#mapping-functions)
  - [Reduction functions](#reduction-functions)
  - [Fold and scan Functions](#fold-and-scan-functions)
- [Creating Streams](#creating-streams)
- [Non-Member Functions for Streams](#non-member-functions-for-streams)
  - [Alternative Generating Tactics](#alternative-generating-tactics)
  - [Prepend Values](#prepend-values)
  - [Zipping Streams](#zipping-streams)
- [Member Functions for Streams](#member-functions-for-streams)
  - [List Processing Functions](#list-processing-functions)
  - [Filtering Function](#filtering-function)
  - [Mapping function](#mapping-function)
- [Conclusion](#youre-done)

We begin with a swift introduction to C++ Collections.
Our goal is to show the main elements of this library as quickly as possible.
Rather than getting caught up in details, rules, exceptions - we want to allow users to make useful programs as quickly as possible.

Unlike the standard design documentation, which provides a list of all methods available and their uses, we prefer brevity over depth here.
This is completely intentional, and we encourage more experienced programmers to refer to our design document to cement their understanding of C++ Collections.

## Getting Started

In order to run C++ Collections, [download][repo_download] and `#include cpp_collections.h`.

You can then start by creating either a Collection or a Stream object.
The difference between a Collection and a Stream object is:

- Collections are traditional functional abstractions on lists are used to define finite lists.
- Streams are self-referential lazily-evaluated data structures that allow for the definition of infinite lists.

## Creating Collections

(We assume that we are using namespace `std` for all examples.)

The Collection is used to represent a finite list. Let's create an empty one.

```cpp
auto empty_collection = Collection<int>();
```

With this empty collection we can demonstrate a number of member functions.
Let's call the `size()` method.

```cpp
int x = empty_collection.size();

std::cout << x << std::endl
// 0
```

Now, rather than instantiating an empty list, we can instantiate a list with a predetermined size.

```cpp
auto presized_collection = Collection<int>(5);

std::cout << presized_collection << std::endl;
// [0,0,0,0,0]
```

This can be validated immediately by running the following:

```cpp
std::cout << presized_collection.size() << std::endl;
// 5
```

Because Collections use a `std::vector` to store its data internally, the `size()` method of a Collection simply calls `size()` on its internal `std::vector`.
Moreover, this implementation detail means that we can easily create a Collection from a `std::vector`.

```cpp
vector<int> v {1,2,3,5,7};
auto vector_collection = Collection<int>(v);
```

In addition to `std::vector`, we can also create a Collection from a `std::list`.

```cpp
list<int> l = {1,2,3,4};
auto list_collection = Collection<int>(l); 
```

We can also use `std::array` and C-style arrays.

```cpp
array<int, 6> foo  = {1, 1, 2, 3, 5, 8};
auto array_collection = Collection<int>(foo);  // std::array

int foo [5] = { 16, 2, 77, 40, 12071 };
auto c_array_collection = Collection<int>(foo, 5); // C-style array

```

As mentioned above, the Collection class uses a `std::vector` to store data internally.
This allows Collections to not only remain fast, but reliable.
Although we use integers for most of our examples here, the Collection can handle any type due to the type agnosticism of vectors. 

## Non-Member Functions for Collections

Although the Collection constructor provides a simple way to instantiate a Collection, the C++ Collections library contains several non-member functions that make this process easier.

If you want to return a Collection of numeric types over the range `[0, size)` use the `range()` function.
Note that the Collection returned by `range()` contains whatever type you passed to `range()` initially.

```cpp
auto a = range(5);
cout << a << endl;

// [0,1,2,3,4]


auto f = range(float(5));
cout << f << endl;

// [0.0,1.0,2.0,3.0,4.0]
```

`range()` can also be used with lower and upper bounds, returning a Collection over the range `[low, high)`.

```cpp
auto b = range(5, 10);
cout << b << endl;

// [5,6,7,8,9]
```

We can also concatenate an arbitrary number of Collections together by using the `concat()` function.

```cpp
auto a = range(5);
auto b = range(5,10);
auto c = range(5);
cout << concat(a, b, c) << endl;

// [0,1,2,3,4,5,6,7,8,9,0,1,2,3,4]
```

Need to use multiple Collections together for an operation?
The `zip()` function combines multiple Collections into a single Collection of tuples.
A [tuple](http://www.cplusplus.com/reference/tuple/) is a dynamically created object that can hold multiple different types.

```cpp
auto a = range(5);
auto b = range(5,10);
auto ab = zip(a, b);

cout << ab << endl;
// [(0,5), (1,6), (2,7), (3,8), (4,9)]
```

Tuples can also hold elements with different types, meaning that `zip()` can be called on Collections of different types as well.
In the following example we see that `zip()` returns a new Collection of tuples that include an `int`, `float`, and `char`.

```cpp
auto e = range(3);
auto f = range(float(3.0));
auto g = Collection(std::vector<char> {'e','f','g'});

auto efg = zip(e, f, g);
cout << efg << endl;
// [(0,0.0,'e'), (1,1.0,'f'), (2,2.0,'g')]
```

`zipWith()` generalizes the functionality provided by `zip()` by allowing us to pass any function to be applied to the members of the zipped lists.
Here we zip two Collections together, *with* a lambda function that adds the members of the Collections together.

```cpp
auto a = range(3);
auto b = range(3);
auto sums = zipWith([](int x, int y) { return x+y; }, a, b)

std::cout << sums << std::endl;
// [0,2,4]
```

## Simple Member Functions for Collections

### Returning Collections

Now that we understand the different ways we can create a Collection, how can we return the data from a Collection?
Since Collections are stored as vectors, the `vector()` method simply returns the internal data object.

```cpp
// returns a std::vector of size 5 with elements 0,1,2,3,4 
auto c = range(5);
std::vector<int> v = c.vector();
```

What happens if we want to return a list?

```cpp
// returns a std::list of size 5 with elements 0,1,2,3,4 
std::list<int> l = Collection<int>(5).list();
```

### List Processing

For processing Collections, the C++ Collections library exposes a set of operations that are common in functional langauges.
Firstly, in order to maintain consistency with the list data structures of Haskell and Scala, we implement the `head()`, `last()`, `init()`, and `tail()` functions.

The `head()` function returns the first element in the Collection.
The `last()` function returns the last element in the Collection

```cpp
auto col = range(1,11);

cout << col.head(); << endl;
// 1

cout << col.last(); << endl;
// 10
```

The `init()` function returns a new Collection with all elements except the last.
The `tail()` function returns a new Collection with all elements except the head.

```cpp
auto col = range(1,11);
cout << col.init() << endl;

// [1,2,3,4,5,6,7,8,9]
```

What would happen if we called `init()` on our Collection *before* printing it?

```cpp
auto col = range(1,11);

col.init()
cout << col << endl;
// [1,2,3,4,5,6,7,8,9,10]

auto new_col = col.init()
cout << new_col << endl;
// [1,2,3,4,5,6,7,8,9]
```

This example might be confusing initially, but the concept it illustrates is central to functional programming. 
`init()` does not mutate the internal `std::vector` of the Collection is is called on but instead returns a new Collection with a new `std::vector` inside.
In fact, both `init()` and `tail()` return modified *copies* of the original Collection object, as do `zip()`, `zipWith()`, and other, more advanced functions we will see a little later.

This means that we can use all these functions together to do create arbitrarily complex Collections.
Here, we create a list of ten odd numbers.

```cpp
auto s = range(11);
s = zipWith([](int x, int y) {return x+y;}, s, s.tail());
std::cout << s << std::endl;

// [1,3,5,7,9,11,13,15,17,19]
```

## Advanced Member Functions for Collections

Now that we have covered basic member functions of Collections we can move on to more advanced member functions.

C++11 lambdas allow us to pass functions as parameters, giving us lots of interesting functionality for advanced member functions.
One of the most basic examples of this is the `each()` function, which allows us to pass a function to be executed once for every element in the Collection.

```cpp
int sum = 0;
auto a = range(5);
a.each([&](int x) {
    sum += x;
});
std::cout << sum << std::endl;

// 10
```

The `filter()` function is similar to the `each()` function, except that the function passed to it must return a boolean value.
`filter()` will test each element with this predicate function and return a subset of the original Collection with the elements that pass the predicate.
Similar to the example above, let's attempt to print a list of odd numbers with `filter()`.

```cpp
auto s = range(20);
s.filter([](int x) { return x % 2 != 0; }).print();

// [1,3,5,7,9,11,13,15,17,19]
```

Let's assume we only want the fourth and fifth odd numbers in our Collection.
The `slice()` method returns a new Collection with the elements whose indices are within the range `[low, high)`.

```cpp
auto s = range(20);
std::cout << s.filter([](int x) { return x % 2 != 0; }).slice(3,5).print();

// [7,9]
```

### Mapping functions

One of the key concepts in functional programming is the `map()` function.
`map()` allows us to apply an arbitrary transformation (passed as a C++11 lambda) to all the elements of the Collection.

```cpp
auto a = range(3);
a.map([](int x) { return x+1; }).print();

// [1,2,3]
```

The `tmap()` function achieves the same effect as `map()`, but uses multiple concurrent `std::threads` to speed up processing.
You can pass the number of threads you would like to execute your transformation with as the second argument to `tmap()` (default is set to `std::thread::hardware_concurrency()`, or, if that returns 0, 4).

```cpp
auto a = range(3);
std::cout << a.tmap([](int x) { return x+1; }, 3) << std::endl;

// [1,2,3]
```

All functions that are prefixed with the character 't' are multithreaded and can be passed the number of threads to execute with as their final parameter.

### Reduction functions

In an example above, we used the `each()` function with an external variable to sum the values in a Collection.
This was not the traditionally functional way of achieving this.
Reduction functions return a single value which is the result of the application of the same binary operator on adjacent pairs of elements in the Collection.
`reduceLeft()` applies the binary operator starting from the left, and `reduceRight()` applies the binary operator starting from the right (or last element).

Here we use `reduceLeft()` to find the sum of the integers 0 - 4. 

```cpp
int sum = range(5).reduceLeft([](int x, int y) { return x+y; });
std::cout << sum << std::endl;

// 10
```

It is worth diving into this a little bit more, and inspecting the state of the Collection at each step of the reduction.

```cpp
begin : 0,1,2,3,4
step 1:   1,2,3,4
step 2:     3,3,4
step 3:       6,4
step 4:        10
```

Reduction functions move pairwise across the Collection, applying the same binary operator on each pair that it finds. 

Similar to `tmap()`, we also have `treduce()`, an alternative implementation of reduce that uses multiple concurrent threads to speed up processing.
Note that the function passed to `treduce()` must be commutative to achieve accurate results, becuase we cannot guarantee that all binary reductions will occurr in the expected order when working with multiple threads.

```cpp
int sum = range(5).treduce([](int x, int y) { return x+y; }, 3);
std::cout << sum << std::endl;

// 10
```

### Fold/Scan Functions

We have two fold functions: `foldLeft()` and `foldRight()`.

This returns the result of the application of the same binary operator on all elements in the Collection as well as an initial value, starting from the left.
Folds are just the same as reductions, except that they start with an initial value, meaning that a fold can return a type different from that stored in the original Collection, while a reduction cannot.

```cpp
int sum = range(5).foldLeft([](int x, int y) { return x+y; }, 0);
std::cout << sum << std::endl;

// 10
```

In addition to the fold functions, we can also return the intermediate results of the binary accumulation of elements in a Collection with `scanLeft()` and `scanRight()`.

```cpp
auto ints = range(1,5);
auto ints2 = ints.scanLeft([](int x, int y) { return x+y; }, 1);
std::cout << ints2 << std::endl;

// [1,2,4,7,11]
```

## Creating Streams

Creating Streams is a different from creating Collections.
The key differentiating factor between Streams and Collections is that Streams can be used to represent potentially infinite lists of data.
These infinite Streams can then be converted to finite Collections using the `take()` function. 

First, let's create an infinite Stream of 1s.

```cpp
std::function<Stream<int>()> ones_gen = [&]() { return Stream<int>(1, ones_gen) };
Stream<int> ones = Stream<int>(1, ones_gen);
```

Let's break down what is happening here. 
Streams are self-referential data structures, meaning that in addition to the head, or the first value in the list, the class stores a pointer to a function that returns another Stream.
Here, we define a function `ones_gen()` that return a new Stream with a head of 1 and a generator function `ones_gen()`.
We then construct the new Stream `ones` with the default Stream constructor, passing the initial head value of 1, and the generator function `ones_gen()`.

Now, we can take an arbitrary number of elements out of this Stream with the `take()` function.

```cpp
std::cout << ones.take(3) << std::endl;

// [1,1,1]
```

Note that `take()` returns a Collection, so we could also use any of the Collection manipulation methods on the result.
The `take()` method converts the Stream into a Collection by repeatedly requesting the head of the Stream, and if it doesn't exist, returning the head of the Stream constructed by the generator function.

Let's try a more complex example.
Here we generate the Fibonacci sequence.

```cpp
std::function<Stream<int>(int, int)> fibs = [&](int prev, int curr) -> Stream<int> {
    return Stream<int>(curr, [=]() -> Stream<int> {
        return fibs(curr, prev + curr);
    });
};

Collection<int> tenfibs = fibs(0, 1).take(3);
```

The Stream generator `fibs`, when called with arguments `0` and `1`, creates a new Stream that has a head value of 1 and an internal pointer to the function `fibs(1, 1)`.
Then, when we ask to `take(3)` elements from the Stream, the structure returns its head, and then lazily computes the remainder of the elements it needs to satisfy our request.
It does this by returning the result of its stored function pointer.
In this case, `tenfibs` would first return 1 becuase that is its head.
It would then return the head of the Stream that results from the evaluation of the function it stores a pointer to, namely `fibs(1, 1)`, which is 1.
The result of `fibs(1, 1)` also stores a pointer to another function, `fibs(1, 2)`.
To find the third and final value, the Stream will evaluate this function and return the resulting Stream's head, which is 2.
For a step-by-step visual depiction of the underlying state of our Stream during the `take(3)` function call, see the graphic below:

```
tenfibs.head      tenfibs.gen
     1             fibs(0,1)
     1             fibs(1,1)
     2             fibs(1,2)
     3             fibs(2,3)
```

## Non-Member Functions for Streams

### Alternative Stream Creation Tactics

Unfortunately, both examples above are rather verbose. 
C++11 does not allow the definition of recursive lambdas, so we have to define our Stream generators as `std::function` intead.
This creates a lot of syntax overhead to the definition of arbtrary Stream generator functions.
In order to ease the syntactic pain of this process, we implemented a series of helper functions to make this process more straightforward.

`repeat()` is the simplest of these functions. It creates an infinite Stream of whatever value it is passed.

```cpp
repeat(1).take(5).print();

// [1,1,1,1,1]
```

The `from()` function takes two parameters, an initial value, and a step. 
It then constructs a Stream starting at the initial value, incrementing by the step each time.
If the step is omitted from the function call, it defaults to 1.

```cpp
from(1).take(5).print();

// [1,2,3,4,5]
```

```cpp
from(1,2).take(5).print();

// [1,3,5,7,9]
```

`iterate()` allows users to define an infinite Stream in terms of an initial value `x`, and a function `f`.
The Stream is then constructed as `x, f(x), f(f(x)), f(f(f(x)))...`.

```cpp
iterate([](int x) { return x*x; }, 2).take(5).print();

//


```

### Prepend Values

Assume you want to add values to a Stream. 
`cons(T value, Stream<T> other)` prepends a value to a Stream.

```cpp
auto ints = from(1);
std:cout << cons(10, ints).take(5) << std::endl;

// [10,1,2,3,4]
```

This can also be done with the `&` operator.
However, note that parentheses must be used appropriately because the operator is not defined as a member operator.

```cpp
auto ints = from(1);
std:cout << (10 & (20 & ints)).take(5) << std::endl;

// [10,20,1,2,3]
```


### Zipping Streams

`zip()` returns a Stream of tuples, where each tuple contains the elements of the zipped Streams that occur at the same position.
`zipWith()` generalizes `zip()` by taking the function for zipping as the first argument instead of a tupling function.

These methods work in the exact same way as their Collection counterparts, except that they operatee on Streams instead.

## Member Functions for Streams

### List Processing Functions

Similar to Collections, we can call `head()` to return the first element of the Stream.

```cpp
std::cout << from(1).head() << std::endl;

// 1
```

`tail()` returns the Stream without the current head.

```cpp
std::cout << from(1).tail().head() << std::endl;

// 2
```

### Filtering Function

Just like Collections, we can also `filter()` Streams.
`filter()` simply returns a sub-Stream of elements of the Stream that match the predicate function provided.

```cpp
auto odds = from(1).filter([](int x) { return x % 2 != 0; }).take(5);
std::cout << odds << std::endl;

// [1,3,5,7,9]
```

### Mapping function

Again, like Collections, `map()` allows us to return a Stream transformed according to the function we pass it.

Let's create a list of the first five squares.

```cpp
auto squares = from(1).map([](int x) { return x * x; });
std::cout << squares.take(5) << std::endl;

// [1,4,9,16,25]
```

## You're Done!
You've made it!
Thank you so much for reading through our tutorial highlighting the features, syntax, and code that drives our library.
By piping together the Collection and Stream methods you find here, you will be able to easily write complex, functional list operations in C++ with readability of a language like Scala.

We hope that C++ Collections will make functional programming concepts with both finite and infinite data structures more accessible and understandable to C++ programmers.
If you would like to contribute to the project it can be found [here][2]. 

Thank you and stay posted for updates.

<!-- Sources -->
[repo_download]: https://github.com/natebrennand/cpp_collections/archive/master.zip
[2]: https://github.com/natebrennand/cpp_collections/

