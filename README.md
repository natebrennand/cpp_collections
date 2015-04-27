Download
--------

[C++ Collections][dl] (18 KB) -- 10 April 2015

[dl]: https://github.com/natebrennand/cpp_collections/archive/master.zip


----
## Introduction

Collections are fundamental to most programming tasks because they allow the user to group and process large sets of data. 
However, when it comes to expressing moderately complex collection manipulations, C++ is markedly behind some of its more modern counterparts with respect to code clarity and efficiency of space. 
This is the problem we set out to solve.

![Image of Kesiev](http://i.imgur.com/GnCHqIm.png?1) 

Languages are used as tools to communicate, and consequently, the structure and limitations of the languages we use determine the way we think. 
This idea, known in the field of linguistic relativity as the [Sapir-Whorf hypothesis][2], has critical implications when applied to programming languages, namely that the ability of a programmer to reason about a problem can be limited by the languages he or she has learned (or has yet to learn).
Furthermore, if we hold this hypothesis to be true, we can reasonably conclude that learning how to write code in a new language can provide a programmer with a totally new way of thinking about solving a problem that he or she may have already solved a dozen times before.

C++ is an interesting language in this respect because in recent years it has begun to introduce new language constructs that help it blur the lines between programming paradigms along which programming languages are usually divided. 
Perhaps most notably, C++11 introduced a set of features that allows for functional programming in the language.
It is as if the English language introduced a new group of words, whose meanings were all missing from the original dictionary.
The conclusion we are tempted to draw from this addition is that C++ is a great language to learn, because its new functional vocabulary will allow the programmer who learns it to think about problems both from a traditional C++ perspective, and now also from a functional perspective.

Unfortunately, the ease of use of many of C++'s functional features pales in comparison to that of other functional languages, making it daunting for beginners in functional programming to use the features correctly, if at all.

> In 24 hours you might be able to learn some of the syntax of C++ (if you already know another language), but you couldn't learn much about how to use the language. In short, if you were, say, a Basic programmer, you could learn to write programs in the style of Basic using C++ syntax, but you couldn't learn what C++ is actually good (and bad) for. So what's the point? Alan Perlis once said: "A language that doesn't affect the way you think about programming, is not worth knowing" -- [Peter Norvig][1]

[1]:http://www.norvig.com/21-days.html

[2]:http://en.wikipedia.org/wiki/Linguistic_relativity

C++ Collections is a library built on top of C++11 that provides both finite and infinite collections data structures, built from the ground up with syntax in mind.
It strives to make functional programming concepts, specifically to do with lists, more accessible and understandable to C++ programmers.

To use the C++ Collections library, download it from Github and simply *#include cpp_collections.h* in your source file.

----
## Collections
C++ Collections provides traditional, functional abstractions on lists.

The Collection class is used to represent any finite list. 

Assume you would like to create a list of numbers from 1 to 100, sum them, and then store the value in an `int`.
The Collection class allows you to achieve this in one line of code.

```cpp
int sum = range(1, 101).reduceLeft([](int x, int y) {return x + y;});
```

Note the similarity to the syntax provided by functional languages like Haskell and Scala.

```haskell
sum = foldl1 (+) [1..100]
```

```scala
val sum = Range(1, 101).reduce((a,b) => a+b)
```

### Implementation Details: Collections

The Collection class uses a `std::vector` to store data internally.
Consequently, Collections are very fast and reliable.
They are also fully type-generic and can be efficiently constructed from a variety of existing STL data structures, including vectors, lists, arrays, and C-style arrays.
What differentiates the Collection from the `std::vector` is mainly the functions defined on top of it, such as `map`, `zip`, `reduce`, and `fold`.
Despite the movement of modern C++ toward functional programming, these traditional functional methods are generally absent from the language.
If they are present, they exist as generalized STL functions, not member functions of existing data structures (see `std::accumulate`).

----
## Streams

The Stream class is used to represent potentially infinite lists of data. 
This is a fairly common concept in functional programming languages but is rare in C++. 
Streams can be converted to finite Collections via the `take()` function.

Observe the following example, again computing the sum of the numbers 1 to 100.

```cpp
Stream<int> ints = from(1);
Collection<int> ints_1_100 = ints.take(100);
int sum = ints_1_100.reduceLeft([](int x, int y) {return x + y;});
```

...or...

```cpp
int sum = from(1).take(100).reduceLeft([](int x, int y) {return x + y;});
```

Here, we define the infinite Stream with the `from()` function, which by default returns a Stream of elements of its argument's type where each element is incremented by 1. 
We then use the `take()` function to convert our Stream into a finite Collection.
We then reduce over this Collection with a function.

Note that Streams can be arbitrarily complex.
Below, we define a Stream to generate the Fibonacci sequence with the help of one of the macros provided by the C++ Collections library.

```cpp
def_generator(fibs, int, int prev, int curr) {
    return Stream<int>(curr, [=]() -> Stream<int> {
        return fibs(curr, prev + curr);
    });
};

std::cout << fibs(0, 1).take(10) << std::endl;

>>> [1,1,2,3,5,8,13,21,34,55]
```

### Implementation Details: Streams

The Stream class is a self-referential data structure, meaning that in addition to the head, or the first value in the list, the class stores a pointer to a function that returns another Stream at all times.
Recall the definition of the Fibonacci Stream generator from above.
Written without the macro, the definition is as follows:

```cpp
std::function<Stream<int>(int, int)> fibs = [&](int prev, int curr) -> Stream<int> {
    return Stream<int>(curr, [=]() -> Stream<int> {
        return fibs(curr, prev + curr);
    });
};

Stream<int> tenfibs = fibs(0, 1).take(3); 
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

----
## Pipelines
The Collection and Stream classes are exciting because they allow C++ developers to construct complex pipelines of functions.
These functions consist of of three main types:
  1. Source operators: Methods that instantiate or create a Collection or Stream. 
  2. Intermediate operators: Methods that take a Collection or Stream as input and output a new Collection or Stream.
  3. Terminal operators: Methods that take a Collection or Stream as input and output something that is not another Collection or Stream.

Pipelines can be formed by combining a source operator, one or more intermediate operators, and a terminal operator.
For example, summing the squares from 1 to 100 can be easily represented by the following pipeline:

```cpp
int sum = range(1, 101).map([](int x) { return x*x; }).reduceLeft([](int x, int y) { return x+y; });
```

Here, `range` is the source operator, `map` is an intermediate operator, and `reduceLeft` is a terminal operator.
Note how these pipelines can become arbitrarily complex by adding more intermediate operators.

----
### Collections Methods

#### Collection<T>()

Construct an empty Collection. 

*Example:*
```cpp
auto empty_collection = Collection<int>();
```

#### Collection<T>(int size)

Construct a presized Collection.

*Example:*
```cpp
auto presized_collection = Collection<int>(5);
```

#### Collection<T>(std::vector<T> list)

Construct a Collection from a `std::vector`.

*Example:*
```cpp
auto vector_collection = Collection<int>(std::vector<int> {1, 2, 3});
```

#### Collection<T>(std::list<T> list)

Construct a Collection from a `std::list`.

*Example:*
```cpp
auto list_collection = Collection<int>(std::list<int> {1, 2, 3});
```

#### Collection<T>(std::array<T, size> list)

Construct a Collection from a `std::array`.

*Example:*
```cpp
auto array_collection = Collection<int>(std::array<int,3> {1, 2, 3});
```

#### Collection<T>(T d[], int len)

Construct a Collection from a C-style array (requires length).

*Example:*
```cpp
int int_c_array[] {1, 2, 3};
auto c_array_collection = Collection<int>(int_c_array, 3);
```

#### concat()

Return the Collection that results from the concatentation of an arbitrary number of Collections of the same type.

*Example:*
```cpp
auto a = range(3);
auto b = range(3);
auto c = range(3);
std::cout << concat(a, b, c) << std::endl;

>>> [0,1,2,0,1,2,0,1,2]
```

#### range(T size)

Return Collection of numeric types over the range `[0, size)`.

*Example:*
```cpp
auto a = range(5);
std::cout << a << std::endl;

>>> [0,1,2,3,4]
```

#### range(T low, T high)

Return Collection of numeric types over the range `[low, high)`.

*Example:*
```cpp
auto a = range(5, 10);
std::cout << a << std::endl;

>>> [5,6,7,8,9]
```

#### zip(Collection<U>...)

Return a Collection of tuples, where each tuple contains the elements of the zipped Collections that occur at the same position.

*Example:*
```cpp
auto a = range(3);
auto b = range(3.0);
auto c = Collection(std::vector<char> {'a','b','c'});

auto d = zip(a, b, c);

assert(d[0] == std::make_tuple(0, 0.0, 'a'));
```

#### zipWith(Function func, Collection<U>...)
Generalizes `zip` by zipping with the function given as the first argument instead of a tupling function.

*Example:*
```cpp
auto a = range(3);
auto b = range(3);

std::cout << zipWith([](int x, int y) { return x+y; }, a, b) << std::endl;

>>> [0,2,4]
```

#### Collection\<T\>::init()

Return all elements except the last.

*Example:*
```cpp
auto a = range(5);
std::cout << a.init() << std::endl;

>>> [0,1,2,3]
```

#### Collection\<T\>::tail()

Return all elements except the head.

*Example:*
```cpp
auto a = range(5);
std::cout << a.tail() << std::endl;

>>> [1,2,3,4]
```

#### Collection\<T\>::pop_head()

Remove the head of the collection.

*Example:*
```cpp
auto a = range(5);
a.pop_head();
std::cout << a << std::endl;

>>> [1,2,3,4]
```

#### ::each()

#### ::filter()

#### ::slice()

#### ::map()

#### ::tmap()

#### ::reduceLeft()

#### ::reduceRight()

#### ::treduce()

#### ::foldLeft()

#### ::scanLeft()

#### ::scanRight

### Terminal operators

#### ::vector()

**Method**:
```cpp
std::vector<T> vector();
```

Returns the Collection as a vector object.
Since Collections are stored as vectors, this method simply returns the internal data object.

*Example*:
```cpp
std::vector<int> v = Collection<int>(5).vector(); //returns a vector of 5 zeros
```

#### ::list()

**Method**:
```cpp
std::list<T> list();
```

Returns the Collection as a list object.
Since Collections are stored as vectors, this method simply returns the internal data object which is then converted to a list by using <section>std::begin(Data)</section> and <section>std::end(Data)</section> to a <section>std::list</section>.

*Example*:
```cpp
std::list<int> l = Collection<int>(5).list(); //returns a list of 5 zeros
```

#### ::size()

**Method**:
```cpp
int size();
```

Returns an `int` with the current size of the collection. 
This is executed by running the <section>size()</section> method on the internal vector which holds the internal data.

*Example*:
```cpp
auto col = Collection<int>(5);
std::cout<< collec.size() << std::endl;
```

#### ::print()

**Method**:
```cpp
void print();
```

This method is also a terminal operator but does not return anything, but instead prints the Collection to the `std::cout`.

*Example*:
```cpp
auto col = Collection<int>(5);
col.print();
```

*Output*:
```
[0,0,0,0,0]
```

#### ::head()

**Method**:
```cpp
T head();
```

This method is a terminal operator which returns the first value of the Collection.
It returns based on the type of the values stored on the Collection.

*Example*:
```cpp
auto col = range(1,101);
std::cout<< col.head(); << "\n";
```

*Output*:
```
1
```

#### ::last()

**Method**:
```cpp
T last();
```

This method is a terminal operator which returns the last value of the Collection.
It returns based on the type of the values stored on the Collection.

*Example*:
```cpp
auto col = range(1,101);
std::cout<< col.last(); << "\n";
```

*Output*:
```
100
```


----
## Development Support

If you are interested in contributing, please fork our repository and submit a pull request. 

----
## Tests

The test suite is composed of tests to check that we stop all errors at the compilation stage, not at runtime.
Files placed in the `tests/` directory will be compiled against `collections.h`.

Files with `fail_` at the start of the filename will be expected to fail.

Those without will be expected to build.
Buildable files will also be executed.
They should include assertions to insure the library is executing as desired.
Passing tests should not include any output to stdout/stderr.


Executable with `make`:
```
make test
```

### Subsets

A subset of the tests can be run by utilizing the `run_tests.sh` script directly.
If an argument is provided, only the tests matching it will be run.


Ex. Run all tests matching reduce:

```bash
./run_tests.sh reduce
```

  [tfmenu]: /graphics/markdown/mt_textformat_menu.png
