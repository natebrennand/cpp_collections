
## Building a Functional Collections Library in C++

One of our group's favorite facets of C++'s recent development has been its movement toward the adoption of functional design concepts.
After introducing lambda functions in C++11, and functors long before that, C++ has emerged as a viable language in which to write programs that conform to the functional paradigm.
However, when compared with prominent modern functional programming languages like Haskell or Scala, C++ lags behind in several respects.
The objective of our project is to provide a library of simple constructs for manipulating lists and other STL containers in a functional manner through the use of maps, reduces and folds.

One of the areas C++ falls behind many functional languages is conciseness.
Both Haskell and Scala allow for compact, easy to understand list comprehensions that read more like math notation than a programming language.




#### Sum integers from 1 to 100:

```haskell
-- Haskell
sum = foldl1 (+) [1..100]
```

```scala
// Scala:
val sum = Range(1, 100).reduce((a,b) => a+b)
```

```cpp
// C++
int sum = 0; for (int i = 1; i <= 100; i++) sum += i;
```


While C++ is still competitive on simple operations, the power of a functional collections library starts to show when a series of operations are performed on the same sequence of objects.
The C++ example could be made slightly more concise but the code would be less composable and adding or removing a step would be far more involved.




#### Create a list of the square of every number between 1 and 10 then sum them:

```haskell
-- Haskell
sum = foldl1 (+) (map (^2) [1..100])
```

```scala
// Scala:
val sum = (1 to 10).map(x => x * x).reduce((a, b) => a + b)
```

```cpp
// C++
std::vector<int> squares = std::vector<int>(10);
for (int i = 0; i < 10; i++)
  squares[i] = i * i;
int sum = 0;
for (int i: squares)
  sum += i;
```



We plan to create a small but powerful library of macros and functions that will help to solve this problem, and allow users to write C++ list comprehensions that looks concise and declarative, in the style of Scala and Haskell, while maintaining compatibility with STL data structures and allowing simple integration with existing libraries.

Another respect in which C++ falls behind modern languages is the generation of infinite sequences.
Scala and Haskell each have their own constructs for supporting the generation of arbitrarily long lists of values given an open-ended range and a series of filters and transformations.
These values can be theoretically generated until infinity, but in practice are evaluated lazily, only when they are called for.
We want to look into creating a C++ iterator construct that facilitates this kind of lazy list evaluation.




#### Find squares greater than 100 on demand:


```haskell
-- Haskell
take 20 ([x^2 | x <- [1..], x^2 > 100])
```

```scala
// Scala:
val c = Iterator.from(1).map(x => x*x).filter(x => 100 < x)
println(c.next()) // 121
println(c.next()) // 144
```

```cpp
// C++
class squares {
public:
  squares(){ last = 1; }
  int next(){
    int temp = last;
    last += 1;
    return temp*temp;
  }
private:
  int last;
};
```


We hope that between our library of list comprehensions and our iterator construct for generating infinite lists, we can create a subset of C++ that allows for concise, functional manipulation of traditional C++ data structures.

