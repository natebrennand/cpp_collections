
# Tutorial

Index:

- [Getting Started](#getting-started)
- [Creating Collections](#creating-collections)
- [Non-Member Functions for Collections](non-member-functions-for-collections)
- [Simple Member Functions for Collections](simple-member-functions-for-collections)
  - [Returning Collections](returning-collections)
  - [list Processing](list-processing)
- [Advanced Member Functions for Collections](advanced-member-functions-for-collections)
  - [Mapping functions](mapping-functions)
  - [Reduction functions](reduction-functions)
  - [Fold and scan Functions](fold-and-scan-functions)
  - [Extra](extra)
- [Creating Streams](creating-streams)
- [Macros](macros)
- [Non-Member Functions for Streams](non-member-functions-for-streams)
  - [Alternative Generating Tactics](alternative-generating-tactics)
  - [Prepend Values](prepend-values)
  - [Zipping Streams](zipping-streams)
- [Member Functions for Streams](member-functions-for-streams)
  - [List Processing Functions](list-processing-functions)
  - [Filtering Function](filtering-function)
  - [Mapping function](mapping-function)
- [Conclusion](Youre-Done)

We will begin with a swift introduction to C++ Collections.
Our goal is to show the main elements of this library as quickly as possible.
Rather than getting caught up in details, rules, exceptions - we want to allow users to make useful programs as quickly as possible.

Unlike the standard design documentation which provides a list of all methods available and their uses, we may prefer brevity over depth.
This is completely intentional, but due to documentation provided, more experienced programmers can use our design document to cement their understanding of C++ Collections.



## Getting Started

In order to run C++ Collections, [download][repo_download] and `#include collections.` for the Collection class (here forth referred to as Collection with a capital 'C') or `#include streams.h` for the Stream class (here forth referred to as Stream with a capital 'S').

Once you've installed and added the appropriate libraries, you can then start by creating an either Collection or Stream object.
The difference between a Collection and a Stream object is:

- Collections are traditional functional abstractions on lists are used to define finite lists.
- Streams are self-referential lazily-evaluated data structures which allow the storage of the head and a pointer to a function which returns another stream.



## Creating Collections

The Collection is used to represent a finite list which allows a number of functions outside of STL to be called on it.

To create a Collection, use the *auto* keyword and create an empty collection.

```cpp
auto empty_collection = Collection<int>();
```

With this empty collection we have a number of member functions.
Let's call the `size()` method.

```cpp
int x = empty_collection.size();

std::cout << x << std::endl

>>> 0
```

We can assume that we are `using namespace std` for all examples.
Now, rather than instantiating an empty list, we can instantiate a list with a predetermined size.

```cpp
auto presizedCollection = Collection<T>(5);
```

This can be validated immediately by running the following:

```cpp
std::cout<< presizedCollection.size() << std::endl;

>>> 5
```
The `size()` method works because since the internal data object of a Collection is a vector, this simply calls `size()` from `std::vector<T>` on the internal data object.
Now, since the Collection constructor is implemented by abstracting a private `vector`, you can create a Collection using a vector as well.

```cpp
auto vectorCollection = Collection<T>(std::vector<T> d);
```

Rather than just vectors, you can also create a Collection with an array:

```cpp
auto arrayCollection = Collection<T>(std::array<T, SIZE> d); // created with an array
```

You can also use a c-style array:

```cpp
auto cListCollection = Collection<T>(T d[], int len); // C-style array
```

You can even use a list:
```cpp
auto listCollection = Collection<T>(std::list<T> d); // created with a list
```


As mentioned above, the Collection class uses a `std::vector` to store data internally. 
This allows Collections to not only remain fast, but reliable. 
Although we are using integers for most of our examples here, the Collection can handle any type due to the type agnosticism of vectors.
Now we will explore some other ways to create, manipulate, and use Collections.

## Non-Member Functions for Collections

Although the Collection constructor provides a simple way to instantiate a Collection.
Often times, it can be easier to use non-member functions to return collections.
This can be another method of creating collections - particularly with certain constraints.


If you want to return a Collection of numeric types over the range `[0, size)` use the `range()` function.

```cpp
auto a = range(5);
a.print();

>>> [0,1,2,3,4]
```

This uses both the `print()` method and the `range(T size)` to return a Collection from zero to four.
Another way you can utilize `range()` is to use the other set of parameters where you return a collection over the range `[low, high)`.

```cpp
auto b = range(5, 10);
b.print(); //std::cout << arr << std::endl;

>>> [5,6,7,8,9]
```

Again we utilized the `print()` method which allows you to output the contents of a Collection to the `std::out`.
Now assuming you would like to return a Collection that can concatenate with another Collection we can do that the following way.

```cpp
auto a = range(5);
auto b = range(5,10);
auto c = range(5);
concat(a, b, c).print();

>>> [0,1,2,3,4,5,6,7,8,9,0,1,2,3,4]
```

So what if the user wants to return a tuple where each tuple contains the elements of the zipped Collections that occur at the same position?

```cpp
auto d = zip(a, b, c); //using
assert(d[0] == std::make_tuple(0, 5, 0));
```

The assert here will pass because of how zip works. 
Now what if we use three different types?

```cpp
auto e = range(3);
auto f = range(3.0);
auto g = Collection(std::vector<char> {'e','f','g'});

auto h = zip(e, f, g);

assert(h[0] == std::make_tuple(0, 0.0, 'e'));
```

Again, this will work because of how tuples don't need to all have the same type to work properly.
Now if you put a function into the generalize `zip` function instead called `zipWith()` you can return a collection rather than a tupling function.

```cpp
auto a = range(3);
auto b = range(3);
std::cout << zipWith([](int x, int y) { return x+y; }, a, b) << std::endl;

>>> [0,2,4]
```

## Simple Member Functions for Collections

### Returning Collections

Now that we understand the different ways we can create a Collection, how can we return a Collection?
Since Collections are stored as vectors, this method simply returns the internal data object.

```cpp
std::vector<int> v = Collection<int>(5).vector(); //returns a vector of 5 zeros
```
What happens if we want to return a list?
Since Collections are stored as vectors, this method simply returns the internal data object which is then converted to a list by using `std::begin(Data)` and `std::end(Data)` to a `std::list`.

```cpp
std::list<int> l = Collection<int>(5).list(); //returns a list of 5 zeros
```

### List Processing

For list processing, C++ Collections tends to use Haskell method names for returning particular elements on a list.

```cpp
auto col = range(1,101);
std::cout<< col.head(); << std::endl;

>>> 1
```

The `head()` function will return an element of type `<T>` depending on whatever type the Collection originally was. 
This function, however, is not a pipeline operator and instead is a terminal operator as you cannot use dot-syntax to keep adding to it.

```cpp
auto col = range(1,101);
std::cout<< col.last(); << std::endl;

>>> 100
```

Above we have `last()` which similar to `head()`, returns the final element.
The final element is always the one before the element provided in the second parameter as the range function does not include the last parameter in the returned Collection.


The more interesting of the list processing functions are both `init()` and `tail()`.
Both of these also return Collections and can be used on top of each other to produce different mutations of the original Collection.
The two functions are similar with slight differences - `init()` returns a Collection with all elements except the last.

```cpp
auto col = range(10);
col.init().print();

>>> [0,1,2,3,4,5,6,7,8]
```

Now what would happen if we split the two methods `init()` and `print()` into into two separate pieces?

```cpp
auto col = range(10);
col.init()
col.print();

>>> [0,1,2,3,4,5,6,7,8,9]
```

This may be confusing initially - but `init()` does not mutate the internal vector but returns a new Collection with a new vector inside. 
In fact both `init()` and `tail()` return modified copies of the Collection object.
Now, if we call `tail()` which returns all the elements except the head in a Collection on `col.init()` what will the result be?

```cpp
col.init().tail().print();

>>> [1,2,3,4,5,6,7,8]
```

Although `tail()` returns a copy without the head of a Collection. 
You can also remove the head of a collection itself by mutating it with `pop_head()`.

```cpp
auto col = range(10);
col.pop_head();
col.print();

>>> [1,2,3,4,5,6,7,8,9]
```

All these functions can be used together to do things like creating a list of ten odd numbers by doing something like this:

```cpp
auto s = range(11);
s = zipWith([](int x, int y) {return x+y;}, s, s.tail());
s.print();

>>> [1,3,5,7,9,11,13,15,17, 19]
```


## Advanced Member Functions for Collections

Now that we have covered standard member functions, we can move to more advanced member functions. 
Lambdas allow us to apply functions as parameters giving us interesting options for member functions.

One of the first things we can do similar to `zipWith()` is to try out the `each()` function.
This function allows you to call a given block once for each element in the Collection that it is being applied to passing that element as a parameter.
```cpp
int sum = 0;
auto a = range(5);
a.each([&](int x) { 
    sum += x;
});
std::cout << sum << std::endl;

>>> 10
```
The `filter()` function is similar to the `each()` function, but instead of returning a terminal value, returns a subset of the original Collection that has passed through a predicate function.
Similar to **4.2** let's attempt to print a list of even numbers with a filter.
Since we are using `filter()` for a smaller ranger it is understandable that we will have less odd numbers since we are not generating odd numbers, but removing even numbers.

```cpp
auto s = range(11);
s.filter([](int x) { return x % 2 != 0; }).print();

>>> [1,3,5,7,9]
```
Now assuming we would want to filter this Collection more - let's say we try to get the indicies within the range `[1, 3)`.
This is where the `slice()` comes in.
```cpp
auto s = range(11);
s.filter([](int x) { return x % 2 != 0; }).slice(1,3).print();

>>> [3,5]
```
This method allows the user to return a subset of the original collection based on provided indicides.

### Mapping functions
One of the key components to functional programming, particularly coming from a language like Ruby or Scala is the map function.
C++11's lambdas become useful in implementing `map()`.
```cpp
auto a = range(3);
a.map([](int x) { return x+1; }).print();

>>> [1,2,3]
```

This function above allows us to apply a transformation to the Collection generated in using `range(3)`. 
Although it may be syntactically similar, `tmap()` allows the user to call the map function using multi-threading.
The use of multiple concurrent `std::threads` speeds up processing.

```cpp
auto a = range(3);
std::cout << a.tmap([](int x) { return x+1; }, 3) << std::endl;

>>> [1,2,3]
```

All functions that have a prefix with the character 't' are multithreaded and have the second parameter holding the number of necessary threads.
Also, std::cout can be used to print output similar to `print()`.

### Reduction functions

Return the final value of the application of the same binary operator on adjacent pairs of elements in the Collection, starting from the left.
The use of reduceLeft can allow binary operators to apply to large lists with a small synatx foot-print.
Assume you want to sum the first five digits from zero to four inclusively:

```cpp
int sum = range(5).reduceLeft([](int x, int y) { return x+y; });
std::cout << sum << std::endl;

>>> 10
```
We can do the same thing reducing right.
This is the same function just operating from the right first:

```cpp
int sum = range(5).reduceRight([](int x, int y) { return x+y; });
std::cout << sum << std::endl;

>>> 10
```

Similar to `tmap()`, we also have `treduce()`: an alternative implementation of reduce that uses multiple concurrent threads to speed up processing.
The function passed to treduce must be commutative to achieve accurate results.

```cpp
int sum = range(5).treduce([](int x, int y) { return x+y; }, 3);
std::cout << sum << std::endl;

>>> 10
```

### Fold/Scan Functions

We have two fold functions: `foldLeft()` and `foldRight()`.

This returns the result of the application of the same binary operator on all elements in the Collection as well as an initial value, starting from the left.

```cpp
int sum = range(5).foldLeft([](int x, int y) { return x+y; }, 0);
std::cout << sum << std::endl;

>>> 10
```

`foldRight()` is similar in terms of function. 
In addition to all these fold functions, we can also return the intermediate results of the binary accumulation of elements in a Collection object starting from the left with `scanLeft()` and then with the right with `scanRight()`.

```cpp
auto ints = range(1,5);
auto ints2 = ints.scanLeft([](int x, int y) { return x+y; }, 1);
std::cout << ints2 << std::endl;

>>> [1,2,4,7,11]
```

### Extra

All in all, the Collection object provides a very clean abstraction over the standard `list` or `vector` object.
The development of many of these functions highlighted above allows tasks like applying binary operations to sets of numbers to be easy.
The multithreading in methods provided also provides a performance boost and is recommended when implementing and using a Collection.

## Creating Streams

Creating Streams is a little different.
One of the key differentiating factors between Streams and Collections is that Streams can be used to represent potentially infinite lists of data. 
Streams, instead can be converted to Collections (our finite data structure) using the `take()` function. 
First, let's create a Stream.
```cpp
std::function<Stream<int>()> ones = [&]() { return Stream<int>(1, ones) };
```
Here we have an infinite list starting at 1. 
It is important that we short-circuit this Stream in order for it to produce some sort of result.
Now if we want to pull out the first three results we can do the following:
```cpp
std::cout << ones().take(3) << std::endl;

>>> [1,1,1]
```
The `take()` method converts the Stream into a Collection for printing.
Prior, however, we use the default Stream constructor.


Let's try and generate the Fibonacci sequence using the standard Stream generator.

```cpp
std::function<Stream<int>(int, int)> fibs = [&](int prev, int curr) -> Stream<int> {
    return Stream<int>(curr, [=]() -> Stream<int> {
        return fibs(curr, prev + curr);
    });
};

Stream<int> tenfibs = fibs(0, 1).take(3);
```

This example is directly from the design document regarding implementation.
To quote the document:
> The Stream generator fibs, when called with arguments 0 and 1, creates a new Stream that has a head value of 1 and an internal pointer to the function fibs(1, 1). Then, when we ask to take(3) elements from the Stream, the structure returns its head, and then lazily computes the remainder of the elements it needs to satisfy our request. It does this by returning the result of its stored function pointer. In this case, tenfibs would first return 1 because that is its head. It would then return the head of the Stream that results from the evaluation of the function it stores a pointer to, namely fibs(1, 1), which is 1. The result of fibs(1, 1) also stores a pointer to another function, fibs(1, 2). To find the third and final value, the Stream will evaluate this function and return the resulting Stream's head, which is 2. - Implementation Details: Streams

We can simply this by using macros to generate Streams rather than depending on `Stream<T>(T head, std::function<T>()> gen)`.

## Macros

A macro is used to ease the syntax of defining any arbitrary recursive Stream generator.
`def_generator()` takes the first parameter providing a name, the second parameter providing a return type, and the rest of the parameters providing arguments for the named function.
```cpp
def_generator(ones, int) {
    return Stream<int>(1, ones)
};
std::cout << ones().take(5) << std::endl;

>>> [1,1,1,1,1]
```
Since streams are recursively defined, this macro syntax provides a clean definition to showing how the `ones()` stream works.
This is more apparent when defining and printing the Fibonacci sequence.
```cpp
def_generator(fibs, int, int prev, int curr) {
    return Stream<int>(curr, [=]() { return fibs(curr, prev+curr); });
};
std::cout << fibs(0, 1).take(10) << std::endl;

>>> [1,1,2,3,5,8,13,21,34,55]
```
## Non-Member Functions for Streams

Non-member functions don't need to act on a Stream to run.

### Alternative Generating Tactics
Now rather than using `def_generator` there is an easier way to generate a Stream using a non-member function called `from()`.
```cpp
from(1).take(5).print();

>>> [1,2,3,4,5]
```
`from()` constructs a Stream starting with the first parameter and incrementing by a step.

### Prepend Values
Now assume you want to add values to a Stream. 
`cons(T value, Stream<T> other)` can allow a user to prepend a value to a Stream.
```cpp
auto ints = from(1);
std:cout << cons(10, ints).take(5) << std::endl;

>>> [10,1,2,3,4]
```
This can even be done with the `&` operator.
```cpp
auto ints = from(1);
std:cout << (10 & (20 & ints)).take(5) << std::endl;

>>> [10,20,1,2,3]
```


### Zipping Streams
Similar to `range()` for Collections, it is easy to use `zip()` and `zipWith()` in the identical manner to the Collections above.
`zip()` returns a Stream of tuples, where each tuple contains the elements of the zipped Streams that occur at the same position.
`zipWith()` generalizes `zip()` by taking the function for zipping as the first argument instead of a tupling function.


## Member Functions for Streams

### List Processing Functions

Now as we showed above with `take()`, there are a couple more key member functions for Streams.
Similar to Collections, we can call `head()` to return the first element of the Stream.

```cpp
std::cout << from(1).head() << std::endl;

>>> 1
```

Similar to Collections, we can do a similar thing by calling `tail()` which will return the Stream without the current head.
Since a Stream is built using it's head, and a pointer to a function generating a value ahead - calling tail() on a Stream like below will produce the following result:

```cpp
std::cout << from(1).tail().head() << std::endl;

>>> 2
```

### Filtering Function

Just like Collections, we can also call a `filter()` function and then convert it to a Collection to print it using `take()`.

`filter()` in this case return a sub-Stream of elements of the Stream that match the predicate function provided.

```cpp
auto odds = from(1).filter([](int x) { return x % 2 != 0; }).take(5);
odds.print();

>>> [1,3,5,7,9]
```

### Mapping function

Similar to Collections as well, `map()` allows us to return a Stream that is given due to the transformation of each element from the original Stream given as input.

Let's create a list of the first five squares.
```cpp
auto squares = from(1);
```
Now we will apply `map()`.
```cpp
auto squares = from(1).map([](int x) { return x * x; });
```
Next, to convert to a Collection we can run:
```cpp
auto squares = from(1).map([](int x) { return x * x; }).take(5);
```
Finally, we can `print()` the five items we need.
```cpp
auto squares = from(1).map([](int x) { return x * x; }).take(5);
squares.print();

>>> [1,4,9,16,25]
```


## You're Done!
You've made it!
Thank you so much for following along our entire guide highlighting all the features, syntax-styling, and code that drives our library.
Now using source operators, intermediate operators, and terminal operators - you can build full pipelines in C++ which provides the readability of a language like Scala.
For example, summing the squares from 0 to 100 is simply:
```cpp
int sum = range(1, 101).map([](int x) { return x*x; }).reduceLeft([](int x, int y) { return x+y; });
```
We hope that C++ Collections will make functional programming concepts with both finite and infinite data structures accessible and understandable to C++ programmers.
If you would like to contribute to the project it can be found [here][2]. Thanks and stay posted for updates.


<!-- Sources -->
[repo_download]: https://github.com/natebrennand/cpp_collections/archive/master.zip
[2]: https://github.com/natebrennand/cpp_collections/

