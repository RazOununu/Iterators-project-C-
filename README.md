vanunuraz@gmail.com
In this ReadMe I learn few features on the internet for an aesthetic format.

# MyContainer Project
**Author:** Raz Oununu  
**Email:** vanunuraz@gmail.com  
**Date:** June 2025  

## Overview
This C++ project implements a generic class MyContainer<T> which manages a dynamic container of elements and provides multiple iteration strategies. The container supports types int, double, and string only (checks at compile-time). The implementation based on C++ features that we learned, such as templates, inheritance, functors, and iterators, and emphasizes safe memory access. 

The project including important concepts:
    **Templates**- to support all the type (int, double, string), I choose to implemant as templates: MyContainer class, IteratorBase class and the iterator classes that inherit from it ,the helper function for test to_vector.
    **Type Safety**– enforced by static_assert.
    **Iterator Inheritance**– all iterators inherit from a IteratorBase class for consistency.
    **Encapsulation of Iteration Orders**– six different traversal on the container.
    **Change counter for Iterator Validity**– before using any iterator, call to compareChanges() and enshure validation.
    **Friend Function**– to support formatted printing, implemant overloading.
    **Comprehensive Testing**– cheaking special cases and all orders tested on few types.

## Project Structure
├── Makefile #Build, test, valgrind and clean targets
├── main.cpp #Demonstration of MyContainer based on Demo.cpp
├── test.cpp #Unit tests with doctest
├── doctest.h #Testing framework
├── MyContainer.hpp #Implementation of MyContainer and all iterators for use on the container. Including separately template of IteratorBase.
└── README.md #This file

## Implementation Details
**Templates**
The class is defined as template <typename T=int> like we were asked but is defined to work only with int, double, or string using a static_assert, checked at compile-time.
Based on principles of templates, all the implementation is in the .hpp file because the compiler needs to see the full definition of a template at the moment of creation, its includes choosing the right template.

**Inheritance and Iterator Design**
All iterators inherit from a common IteratorBase class that stores:
    *A vector with the data.
    *Current index.
    *A pointer to the container's changes counter for invalidation.
This enables uniform iterator behavior and simplifies code reuse for operations like: operator++, operator!=, operator==, operator++(int), operator->, operator*.
Const is applied to operators such as operator* and operator-> to ensure they only provide read access, which enhances safety and enables usage in const contexts (also for ==, !=).

**Functors Usage**
The descending order iterator (DescendingOrder) demonstrates use of std::greater<T>() as a functor to reverse the sorting order.

**Change Tracking Mechanism**
Each iterator stores counter of the container’s changes at the moment it is created.
The container increments this counter (changes++) every time that he modified.
Before each iterator operation, the compareChanges() function compares the counter of the iterator with the current value from the container. If they different, a runtime_error is thrown to indicate that the iterator not valid anymore.
This mechanism enforces safe access and prevents undefined behavior caused by old iterators.

**Friend Function**
The operator<< is implemented as a friend function to allow formatted output of the container’s contents using standard stream syntax (cout << container).
Declaring it as a friend grants access to the container’s private data, which is necessary for printing.

**Iteration Modes Implemented**
Each iterator reuses the current data and is isolated from changes to the container unless created again.
Each iterator has begin_* and end_* methods for traversal. These are the six modes:

| Name             | Behavior                                                         |
|------------------|------------------------------------------------------------------|
| AscendingOrder   | Sorts the elements in increasing order                           |
| DescendingOrder  | Sorts in decreasing order using std::greater<T>()                |
| ReverseOrder`    | Iterates in reverse insertion order                              |
| Order            | Iterates in insertion order                                      |
| SideCrossOrder   | Switch between smallest and largest remaining elements           |
| MiddleOutOrder   | Starts from the middle and alternates outward take care even/odd |

## Testing
The tests use the doctest framework to validate:
    **Basic operations of container**– insertion, deletion, size, exception throwing. Also ensure duplicate elements are correctly handled and all of them removed.
    **Stream output**– formatting of int, double, string.
    **All iterator modes**– including correct element ordering. All iterators were tested for different types.
    **Special cases**– even/odd sized containers, single element, empty container.
    **Invalidation**– using old iterators after changes return exceptions.
    **Iterator comparison (==, !=)**- necessary for loop and valid range.
    **Large container**– iteration over 1000 elements.
    **Runtime errors**- occur if old iterators are accessed after changes like I want.
    **Iterator size**- matches container size, so the iterator not add/remove elemants.

## Compilation
The project is controlled by a Makefile with targets, and all compiled with -std=c++20 -Wall -g flags:
    **make Main**– compiles and runs main.cpp (Demo)
    **make test**– runs unit tests with doctest
    **make valgrind**– checks for memory leaks
    **make clean**– deletes all compiled outputs

