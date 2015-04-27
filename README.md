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

The Stream class is used to represent potentially infinite sets of data. 
This is a fairly common concept in functional programming languages but is rare in C++. 

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
We can then reduce over this Collection with a function.

Note that Streams can be arbitrarily complex.
Below, we define a Stream to generate the Fibonacci sequence with the help of a one of the macros provided by the C++ Collections library.

```cpp
def_generator(fibs, int, int prev, int curr) {
    return Stream<int>(curr, [=]() -> Stream<int> {
        return fibs(curr, prev + curr);
    });
};

std::cout << fibs(0, 1).take(10) << std::endl;

>>> [1,1,2,3,5,8,13,21,34,55]
```

### Fluent Interfaces
[Martin Fowler][mf] coined the term **fluent interface** as a semantic facade that allows you to apply multiple properties to an object without having to redefine the object each time.
Typically they are used to build domain specific languages, fluent interfaces provide not only more discoverable and readable code - but an easy structure to use particularly for people who may not be familiar with working with functional ideas in C++.
Developing C++ Collections will benefit teams that would like to use method chaining as a replacement for typical collection based operations with syntax and code that is easier to parse and uses method chaining to communicate the actions taking place sequentially - similar to a UNIX pipe.

[mf]:http://martinfowler.com/bliki/FluentInterface.html

This form of method cascading which originated in [Smalltalk][sm] allows a layer of syntactic sugar to be added to an otherwise repetitive piece of code.
Cascading prevents the need to list the object repeatedly as you can have numerous methods communicating exactly what they want to convey all acting on a single object by returning and propagating `this`.
[sm]: http://en.wikipedia.org/wiki/Smalltalk


## Implementation Detail

The C++ Collection class uses `std::vector<T>` to store data internally.
This allows a number of different constructors to transfer information to the vector to control pointer management and data organization.
Additionally, this allows lists, arrays, and C-styled arrays to be stored and mapped to the C++ Collection. 

Within the Collection, there are three main types of operations which are elaborated below: source operators, intermediate operators, and terminal operators.
Source operators allow the creation of Collections.
Intermediate operators work off Collections and return Collections.
Finally, Terminal operators return a non-Collection object or no object in return. 


### Collection operations and pipelines
Collections themselves have three types of operations:
- Source operators: Methods which instantiate or create Collections therefore are considered sources of Collections.
They can be evaluated immediately.

- Intermediate operators: These are Collections that both input and output streams. 
Intermediate operators range between O(1) and O(n) depending on whether they are stateless or not.

- Terminal operators: These are operators that close a Collection and finally cause all of the operations in the pipeline to evaluate and return a non-Collection value. 

All Collection pipelines are formed by a combination of a source operator, an either singular or multiple intermediate operators, and concluded with a terminal operator.
These three operators work together to form a pipeline.
Unlike pipelines in Java 8, our pipelines are executed after each individual function, and therefore we do not need to read the terminal operation before our pipeline is executed. 

Additionally, unlike Java 8, despite syntax similarities, we do not use consumable Collections.
The data of a Collection is statefully stored inside a vector in the Collection called `std::vector<T> Data`. 
A Collection is an abstraction on an `std::vector<T> Data`, so many lazily evaluated function require the use of memoization by increasing memory costs to decrease speed.

As mentioned above, the example where one would slice the first five consecutive integers from a list of 1000 numbers involves a new vector being instantiated internally to transfer the first five data elements of the Collection to a newly created list which gets converted into a Collection.
The theory behind many of the functions utilized for the Collections library will be expanded upon in the documentation below.


Below we will tour through the design, the style, and way different methods function and operate.

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
