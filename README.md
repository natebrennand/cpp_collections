Download
--------

[C++ Collections][dl] (18 KB) -- 10 April 2015

[dl]: https://github.com/natebrennand/cpp_collections/archive/master.zip


----
## Introduction

Collections are fundamental to most programming tasks. More importantly, they let you group and process large sets of data. Whether you  want to create a collection of a financial transactions you want to operate on, or you want to calculate something as summing a list of squares between 0 and 10.

![Image of Kesiev](http://i.imgur.com/GnCHqIm.png?1) 

Currently, processing collections in C++ is not only far from perfect, but is syntax-wise far behind many current functional programming paradigms.

Languages are used as tools to communicate, and incidentally, the structure and limitations of a C++ determine the way we think. Learning how to think and within the context of a language provides one the ability to be a proper user, well at the same time 

> In 24 hours you might be able to learn some of the syntax of C++ (if you already know another language), but you couldn't learn much about how to use the language. In short, if you were, say, a Basic programmer, you could learn to write programs in the style of Basic using C++ syntax, but you couldn't learn what C++ is actually good (and bad) for. So what's the point? Alan Perlis once said: "A language that doesn't affect the way you think about programming, is not worth knowing" -- [Peter Norvig][1]

[1]:http://www.norvig.com/21-days.html

This is the principle of linguistic relativity also known as the [Sapir-Whorf hypothesis][2] shows that the constraints of a language are what most often limit our thought processing. This idea, when applied to programming languages shows how Lisp programmers use macros for everything, Python programmers even in Ruby have code that looks like python.  

[2]:http://en.wikipedia.org/wiki/Linguistic_relativity

One of the key components to developing C++ is learnability or more importantly, how easy is it for users to accomplish basic tasks. 

C++ Collections is a C++ 11 library that provides lazy evaluation and functional transformations of the data to lighten the burden of using functional operations such as map, range, and filter for the user. To use C++ Streams, download them from Github and simply <section>#include collections.h</section> in the header of your file after dragging it in.

----
## What is C++ Collections?
C++ Collections provides abstractions on a set of ordered data. All examples below assume that we are <section>using namespace cpp_collections</section>.
The Class collections is used to support functional style transformations as demonstrated above. Assume you would like to create a list of numbers from 1 to 100, sum them, and then store the value in an int. Functional operators allow these operations to happen in nearly one line of code.

'''cpp
int sum = range(1,101).reduceLeft([](int x, int y) {return x + y;});
'''

The syntax you will note is similar to both Haskell and Scala.

'''haskell
sum = foldl1 (+) [1..100]
'''

'''scala
val sum = Range(1, 100).reduce((a,b) => a+b)
'''

The main abstraction introduced by this library is Collection. The capitalization om the word 'Collection' is important to distinguish it from collection. The Collection is different from standard collections in C++ in that:

1. It is functional in nature. All operations on a Collection produce some form of result by modifying the source in memory. 
2. It is concise. After introducing lambda functions in C++11, and functors long before that, C++ has emerged as a viable language in which to write programs that conform to the functional paradigm. There is no template overhead to calling functions.
3. Laziness seeking. We use laziness to optimize many Collections operations. So calling slice on the first five consecutive numbers will not require all numbers in the list to be examined. 
4. Potentially unbounded. While all collections have a finite size, Collections do not. Short circuiting and infinite Collection by calling <section>head</section> on an infinite Collection. 
[comment]: <> (Hopefully we can get infinite lists to work well)

### Fluent Interfaces
[Martin Fowler][mf] coined the term **fluent inteface** as a semantic facade that allows you to apply multiple properties to an object without having to redefine the object each time. Typically they are used to build domain specific languages, fluent interfaces provide not only more discoverable and readable code - but an easy structure to use particularly for people who may not be familiar with working with functional ideas in C++. Developing C++ Collections will benefit teams that would like to use method chaining as a replacement for typical collection based operations with syntax and code that is easier to parse and uses method chaining to communicate the actions taking place sequentially - similar to a UNIX pipe.

[mf]:http://martinfowler.com/bliki/FluentInterface.html

### Collection operations and pipelines
Collections themselves have three types of operations:
- Source operators: Methods which instantiate or create Collections therefore are considered sources of Collections. They can be evaluated immediately.

- Intermediate operators: These are Collections that both input and output streams. intermediate operators range between O(1) and O(n) depending on whether they are stateless or not.

- Terminal operators: These are operators that close a Collection and finally cause all of the operations in the pipeline to evaluate and return a non-Collection value. 

All Collection pipelines are formed by a combination of a source operator, an either singular or multiple intermediate operators, and concluded with a terminal operator. These three operators work together to form a pipeline. Unlike pipelines in Java 8, our pipelines are executed after each individual function, and therefore we do not need to read the terminal operation before our pipeline is executed. 

Additionally, unlike Java 8, despite syntax similarities, we do not use consumabale Collections. The data of a Collection is statefully stored inside a vector in the Collection called <section>std::vector<T> Data</section>. A Collection is an abstraction on an <section>std::vector<T> Data</section>, so many lazily evaluated function require the use of memoization by increasing memory costs to decrease speed.

As mentioned above, the example where one would slice the first five consecutive integers from a list of 1000 numbers involves a new vector being instaniated internally to transfer the first five data elements of the Collection to a newly created list which gets converted into a Collection. The theory behinnd many of the functions utilized for the Collections library will be expanded upon in the documentation below.



---
## API Reference - Methods

### Generators

#### Collection<T>(std::vector<T> d);
Construct a standard Collection using a vector of any type.

#### Collection<T>(std::array<T, SIZE> d);
Construct a standard Collection using an array of any type.

#### Collection<T>(std::list<T> d);
Constructs a collection with a list as any type.

#### Collection<T>(T d[], int len); 
C-style array constructor. It requires length to use as well to create a collection using an array of any type.

#### Collection<T>(int size);
Construct an empty collection of size 'size'. This Colleciton will return nothing but can be used with <section>Collection<T>.size()</section> to return a size.
*Example*:
'''
std::cout<< Collection<int>(5).size() << std::endl; 
'''

*Output*:
'''
5
'''

#### Collection<T>();
Construct an empty collection. This Colleciton will return nothing but can be used with <section>Collection<T>.size()</section> to return a size of 0.

*Example*:
'''
std::cout<< Collection<int>().size() << std::endl;
'''

*Output*:
'''
0
'''

### Intermediate operators

#### ::init();

**Method**:
'''
Collection<T> init();
'''

This method is an intermediate operator which returns all elements except the last.

*Example*:
'''
auto col = range(1,101);
std::cout<< col.init(); << "\n";
'''

*Output*:
'''
[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99]
'''

#### ::tail();

**Method**:
'''
Collection<T> tail();
'''

This method is an intermediate operator which returns all elements except the first.

*Example*:
'''
auto col = range(1,101);
std::cout<< col.tail(); << "\n";
'''

*Output*:
'''
[2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100]
'''

#### ::pop_head

**Method**:
'''
void pop_head();
'''

This method removes the head by erasing the first element of the interal data vector.

*Example*:
'''
auto col = range(1,101);
col.pop_head();
col.print();
'''

*Output*:
'''
[2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100]
'''

### Terminal operators

#### ::vector();

**Method**:
'''
std::vector<T> vector();
'''

Returns the Collection as a vector object. Since Collections are stored as vectors, this method simply returns the internal data object.

*Example*:
'''
std::vector<int> v = Collection<int>(5).vector(); //returns a vector of 5 zeros
'''

#### ::list();

**Method**:
'''
std::list<T> list();
'''

Returns the Collection as a list object. Since Collections are stored as vectors, this method simply returns the internal data object which is then converted to a list by using <section>std::begin(Data)</section> and <section>std::end(Data)</section> to a <section>std::list</section>.

*Example*:
'''
std::list<int> l = Collection<int>(5).list(); //returns a list of 5 zeros
'''

#### ::size();

**Method**:
'''
int size();
'''

Returns an <section>int</section> with the current size of the collection. This is executed by running the <section>size()</section> method on the internal vector which holds the internal data.

*Example*:
'''
auto col = Collection<int>(5);
std::cout<< collec.size() << std::endl;
'''

#### ::print();

**Method**:
'''
void print();
'''

This method is also a terminal operator but does not return anything, but instead prints the Collection to the <section>std::cout</section>.

*Example*:
'''
auto col = Collection<int>(5);
col.print();
'''

*Output*:
'''
[0,0,0,0,0]
'''

#### ::head();

**Method**:
'''
T head();
'''

This method is a terminal operator which returns the first value of the Collection. It returns based on the type of the values stored on the Collection.

*Example*:
'''
auto col = range(1,101);
std::cout<< col.head(); << "\n";
'''

*Output*:
'''
1
'''

#### ::last();

**Method**:
'''
T last();
'''

This method is a terminal operator which returns the last value of the Collection. It returns based on the type of the values stored on the Collection.

*Example*:
'''
auto col = range(1,101);
std::cout<< col.last(); << "\n";
'''

*Output*:
'''
100
'''


## Support

## Development Suport

If you are interested in contributing, 
```
g++ -std=c++11 -Wall tests/<FILE>.cpp
```

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