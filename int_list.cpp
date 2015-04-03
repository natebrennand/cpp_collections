/*
 * Integer list experiments
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

#define forEach(a) for (auto i : a)

using namespace std;

vector<int> x_in(int a, int b) {
    vector<int> list;
    for (int i = a; i <= b; i++)
        list.push_back(i);
    return list;
}

vector<int> x_in_if(int a, int b, function<bool(int)> cond) {
    vector<int> list;
    for (int i = a; i <= b; i++)
        if (cond(i)) list.push_back(i);
    return list;
}

vector<int> map(function<int(int)> func, vector<int> in_list) {
    vector<int> list;
    for (int i = 0; i < in_list.size(); i++)
        list.push_back(func(in_list[i]));
    return list;
}

int foldl1(function<int(int, int)> func, vector<int> list) {
    int result;
    int a = list[0]; 
    int b = list[1]; 
    for (int i = 2; i <= list.size(); i++) {
        result = func(a, b);
        a = result;
        b = list[i];
    }
    return result;
}

int main() {

    auto sum = [](int a, int b) { return a + b; };

    // range list comprehension
    vector<int> list = x_in(1,10);

    // forEach macro
    forEach(list) {
        cout << i << endl;
    }
    cout << "\n";

    // conditional range list comprehension
    list = x_in_if(1, 10, [](int x) { return x % 2 == 0; });
    forEach(list) {
        cout << i << endl;
    }
    cout << "\n";

    // map function + forEach macro
    forEach(map([](int a) { return a + 1; }, list)) {
      cout << i << endl;
    }
    cout << "\n";

    // sum integers from 1 to 10
    cout << foldl1(sum, x_in(1,10)) << endl;
    cout << "\n";

    // sum squares of integers from 1 to 10
    cout << foldl1(sum, map([](int a) { return a * a; }, x_in(1,10))) << endl;
    cout << "\n";
}
