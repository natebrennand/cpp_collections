Download
--------

[C++ Collections][dl] (18 KB) -- 10 April 2015

[dl]: https://github.com/natebrennand/cpp_collections/archive/master.zip


----
## Introduction

Collections are fundamental to most programming tasks because they allow the user to group and process large sets of data. 
However, when it comes to writing moderately complex collection manipulations, C++ is markedly behind some of its more modern counterparts with respect to code clarity and efficiency of space. 
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

## Implementation Details

The Collection class uses a `std::vector` to store data internally.
Consequently, Collections are very fast and reliable.
They are also fully type-generic and can be efficiently constructed from a variety of existing STL data structures, including vectors, lists, arrays, and C-style arrays.
What differentiates the Collection from the `std::vector` is mainly the functions defined on top of it, such as `map`, `zip`, `reduce`, and `fold`.
Despite the movement of modern C++ toward functional programming, these traditional functional methods are generally absent from the language.
If they are present, they exist as generalized STL functions, not member functions of existing data structures (see `std::accumulate`).

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
```


### Pipelines
The Collection and Stream classes are exciting because they allow C++ developers to construct complex pipelines of functions.
These functions consist of of three main types:
  1. Source operators: Methods that instantiate or create a Collection. 
  2. Intermediate operators: Methods that take a Collection or Collections as input and output a new Collection.
  3. Terminal operators: Methods that take a Collection or Collections as input and output something that is not another Collection.

Collections pipelines can be formed by combining a source operator, one or more intermediate operators, and a terminal operator.
For example, summing the squares from 1 to 100 can be easily represented by the following pipeline:

```cpp
int sum = range(1, 101).map([](int x) { return x*x; }).reduceLeft([](int x, int y) { return x+y; });
```

Here, `range` is the source operator, `map` is an intermediate operator, and `reduceLeft` is a terminal operator.

### Source operators

**EXAMPLE HERE**
#### Collection<T>()
Construct an empty collection.
This Collection will return nothing but can be used with <section>Collection<T>.size()</section> to return a size of 0.

*Example*:
```cpp
std::cout<< Collection<int>().size() << std::endl;
```

*Output*:
```
0
```

#### Collection<T>()

**Method**:
```cpp
auto emptyCollection = Collection<T>();
```

Construct an empty collection of size 'size'. 
This Collection will return nothing but can be used with *Collection<T>.size()* to return a size.

**Method**:
```cpp
auto vectorCollection = Collection<T>(std::vector<T> d);
```

Construct a standard Collection using a vector of any type.

**Method**:
```cpp
auto arrayCollection = Collection<T>(std::array<T, SIZE> d);
```

Construct a standard Collection using an array of any type.

**Method**:
```cpp
auto listCollection = Collection<T>(std::list<T> d);
```

Constructs a collection with a list as any type.


```cpp
auto cListCollection = Collection<T>(T d[], int len);
```

C-style array constructor.
It requires length to use as well to create a collection using an array of any type.

#### Collection<T>(int size)

**Method**:
```cpp
auto presizedCollection = Collection<T>(5);
```

*Example*:
```cpp
std::cout<< Collection<int>(5).size() << std::endl; 
```

*Output*:
```
5
```

#### ::concat()


#### ::range()


#### ::zip()

#### ::zipWith()


### Intermediate operators

#### ::init()

**Method**:
```cpp
Collection<T> init();
```

This method is an intermediate operator which returns all elements except the last.

*Example*:
```cpp
auto col = range(1,101);
std::cout<< col.init(); << "\n";
```

*Output*:
```
[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99]
```

#### ::tail()

**Method**:
```cpp
Collection<T> tail();
```

This method is an intermediate operator which returns all elements except the first.

*Example*:
```cpp
auto col = range(1,101);
std::cout<< col.tail(); << "\n";
```

*Output*:
```
[2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100]
```

#### ::pop_head()

**Method**:
```cpp
void pop_head();
```

This method removes the head by erasing the first element of the internal data vector.

*Example*:
```cpp
auto col = range(1,101);
col.pop_head();
col.print();
```

*Output*:
```
[2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100]
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


## Streams

The second object in the C++ Collections library that we would like to expand on is the Stream object.
Streams are developed by having a head of anytype, a tail which is initially set to a nullptr and a generator function.
The initial stream allows users to run `take()`, `head()`, `tail()`, `filter()`, and `map()` on it.
Yet Streams also allow the development of infinite lists using the `from()` as well.
As the Stream object is developed, the documentation will be updated correspondingly. 

## Development Support

If you are interested in contributing, please fork our repository and submit a pull request. 

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
