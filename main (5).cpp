//vanunuraz@gmail.com
//To run the program I choose use the demo file that provides in the exercise.
//This demo includes creating a MyContainer object, adding elements to it, and then iterating through the elements in various orders using the defined iterators.
//The output of each iteration is printed correctley, like expected.

//The functionality in this file used to implement the MyContainer class with the required iterators.
#include <iostream>
#include "MyContainer.hpp"
using namespace exercise4; //Namespace changed to exercise4 as defined in MyContainer.hpp

int main() {
    MyContainer<int> container;
    container.addElement(7);
    container.addElement(15);
    container.addElement(6);
    container.addElement(1);
    container.addElement(2);

    std::cout << "Size of container: " << container.size() << std::endl; //Size of container: 5

    for (auto it = container.begin_ascending_order(); it != container.end_ascending_order(); ++it) {
        std::cout << *it << ' ';   // 1 2 6 7 15
    }
    std::cout << std::endl;

    for (auto it = container.begin_descending_order(); it != container.end_descending_order(); ++it) {
        std::cout << *it << ' ';  // 15 7 6 2 1
    }
    std::cout << std::endl;

    for (auto it = container.begin_side_cross_order(); it != container.end_side_cross_order(); ++it) {
        std::cout << *it << ' ';  // 1 15 2 7 6
    }
    std::cout << std::endl;

    for (auto it = container.begin_reverse_order(); it != container.end_reverse_order(); ++it) {
        std::cout << *it << ' ';  // 2 1 6 15 7
    }
    std::cout << std::endl;

    for (auto it = container.begin_order(); it != container.end_order(); ++it) {
        std::cout << *it << ' ';  // 7 15 6 1 2
    }
    std::cout << std::endl;

    for (auto it = container.begin_middle_out_order(); it != container.end_middle_out_order(); ++it) {
        std::cout << *it << ' ';  // 6 15 1 7 2
    }
    std::cout << std::endl;

  

    return 0;
}