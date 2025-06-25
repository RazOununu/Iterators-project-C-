//vanunuraz@gmail.com
//This file contains test cases for the MyContainer class and its iterators. Each test checks a different aspect: type safety, basic functionality,
//exception handling, and iteration behavior at special cases.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN //This line is necessary to define the main function for the doctest framework
#include "doctest.h"
#include "MyContainer.hpp"
using namespace exercise4;
using namespace std;

//Basic functionality test: add elements, remove an element, check size, and exception on removing a non-existent element
TEST_CASE("Basic functionality on the container"){
    MyContainer<int> c;
    CHECK(c.size()== 0); //At creation, the container should be emptys
    //Exepted adding int sucssed
    c.addElement(5);
    c.addElement(10);
    c.addElement(1);
    CHECK(c.size()== 3); //After adding three elements, the size should be 3
    c.remove(10); //Removing an element should succeed
    CHECK(c.size() == 2); //After removing one element, the size should be 2
    CHECK_THROWS(c.remove(99)); //Removing a non-existent element should throw an exception
}

//Stream output operator test for int, double, string. Ensures formatting and data printing like expected
TEST_CASE("Print operator"){
    //Printing container with int
    {
        MyContainer<int> c;
        c.addElement(2);
        c.addElement(1);
        stringstream ss;
        ss << c;
        CHECK(ss.str()== "[2, 1]"); //Simple check for int output format
    }
    //Operator << with double
    {
        MyContainer<double> c;
        c.addElement(2.5);
        c.addElement(1.1);
        stringstream ss;
        ss<< c; //Using stringstream to check the output format
        CHECK(ss.str().find("1.1")!= string::npos); //str-> ecsess to data in ss, find-> checks if the string contains the substring
    }
    //Printing container with string
    //First methods to check the output format:
    {
        MyContainer<string> c;
        c.addElement("hello");
        c.addElement("world");
        stringstream ss; //Using stringstream to check the output format
        ss<< c;
        CHECK(ss.str().find("hello")!= string::npos); //str-> ecsess to data in ss, find-> checks if the string contains the substring
    }
    //Another method to check the output format:
    {
        MyContainer<string> c;
        c.addElement("hello");
        c.addElement("world");
        stringstream ss;
        ss<< c;
        CHECK(ss.str()== "[hello, world]"); //Because I know what the output format should be, I can check it directly
    }
}

//Helper function to extract vector from iterators. Simplifies test comparisons and improves readability
template<typename T> //Temple function to handle different types
vector<T> to_vector(auto begin, auto end){
    vector<T> iteration;
    for(auto it= begin; it!= end; ++it)
        iteration.push_back(*it); //Using operator* to dereference the iterator and get the value
    return iteration;
}

//Test the helper function to_vector with iterators because its used in many tests to compare iterators output with expected vector
TEST_CASE("Test helper function to_vector"){
    vector<int> vec= {1, 2, 3};
    auto it1= vec.begin();
    auto it2= vec.end();
    CHECK(to_vector<int>(it1, it2)== vec); //Using the helper function to_vector to convert iterators to vector and compare with expected vector
}

//Test all iterators with some data types for each iterator. Ensures correct order and behavior for ascending, descending, side cross, reverse, order,
TEST_CASE("Iterators on all types container"){
    //Ascending
    SUBCASE("Ascending- int"){
        MyContainer<int> ci;
        ci.addElement(3);
        ci.addElement(1);
        ci.addElement(2);
        CHECK(to_vector<int>(ci.begin_ascending_order(), ci.end_ascending_order())== vector<int>{1,2,3}); //Using helper function to_vector
    }
    SUBCASE("Ascending- double"){
        MyContainer<double> cd;
        cd.addElement(2.2);
        cd.addElement(0.1);
        cd.addElement(5.5);
        CHECK(to_vector<double>(cd.begin_ascending_order(), cd.end_ascending_order())== vector<double>{0.1, 2.2, 5.5}); //Using helper function to_vector
    }

    //Descending
    SUBCASE("Descending- int"){
        MyContainer<int> ci;
        ci.addElement(1);
        ci.addElement(3);
        ci.addElement(2);
        CHECK(to_vector<int>(ci.begin_descending_order(), ci.end_descending_order())== vector<int>{3,2,1}); //Using helper function to_vector
    }
    SUBCASE("Descending- string"){
        MyContainer<string> cs;
        cs.addElement("apple");
        cs.addElement("zebra");
        cs.addElement("monkey");
        CHECK(to_vector<string>(cs.begin_descending_order(), cs.end_descending_order())== vector<string>{"zebra","monkey","apple"}); //Using helper function to_vector
    }

    //SideCross    
    SUBCASE("SideCross- even number of elements, int"){
        MyContainer<int> ci;
        for(int x: {1,2,3,4,6,7}){ //Adding with loop
            ci.addElement(x);
        }    
        CHECK(to_vector<int>(ci.begin_side_cross_order(), ci.end_side_cross_order())== vector<int>{1,7,2,6,3,4}); //Using helper function to_vector
    }
    SUBCASE("SideCross- string"){
        MyContainer<string> cs;
        for(string s: {"a","b","c","d"}){
            cs.addElement(s);
        }    
        CHECK(to_vector<string>(cs.begin_side_cross_order(), cs.end_side_cross_order())== vector<string>{"a","d","b","c"}); //Using helper function to_vector
    }

    //Reverse
    SUBCASE("Reverse- double"){
        MyContainer<double> cd;
        for(double x: {1.1, 2.2, 3.3}){
            cd.addElement(x);
        }    
        CHECK(to_vector<double>(cd.begin_reverse_order(), cd.end_reverse_order())== vector<double>{3.3, 2.2, 1.1}); //Using helper function to_vector
    }

    //MiddleOut
    SUBCASE("Old iterator invalid after container modification") {
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    auto it= c.begin_ascending_order();  //Valid accorting to the count of changes
    CHECK(*it== 10); //Should point to the first element
    c.addElement(30); //Modify the container by adding a new element
    CHECK_THROWS(*it); //Old iterator should not be valid anymore after modification and should throw an exception when dereferenced
    }      

    SUBCASE("MiddleOut- string"){
        MyContainer<string> cs;
        for(string s: {"apple", "banana", "cherry", "date", "fig"}){
            cs.addElement(s);
        }    
        CHECK(to_vector<string>(cs.begin_middle_out_order(), cs.end_middle_out_order())== vector<string>{"cherry", "banana", "date", "apple", "fig"}); //Using helper function to_vector
    }

    //Order
    SUBCASE("Order- int"){
        MyContainer<int> ci;
        ci.addElement(10);
        ci.addElement(20);
        ci.addElement(30);
        CHECK(to_vector<int>(ci.begin_order(), ci.end_order())== vector<int>{10,20,30}); //Using helper function to_vector
    }
    SUBCASE("Order- string"){
        MyContainer<string> cs;    
        cs.addElement("first");
        cs.addElement("second");
        cs.addElement("third");
        CHECK(to_vector<string>(cs.begin_order(), cs.end_order())== vector<string>{"first","second","third"}); //Using helper function to_vector     
    }
}  

//Test begin==end on empty container. Ensures correct handling when no elements exist
TEST_CASE("Iterator on empty container"){
    MyContainer<int> c;
    auto begin= c.begin_ascending_order(); //Point to the beginning of the container
    auto end= c.end_ascending_order(); //Also point to the end of the container
    CHECK(begin== end);
}

//Test duplicates and that remove clears all instances
TEST_CASE("Duplicate insertions"){
    MyContainer<int> c;
    c.addElement(5);
    c.addElement(5);
    CHECK(c.size()== 2);
    c.remove(5);
    CHECK(c.size()== 0); //Expecting size to be 0 after removing all instances of 5
}

//Test all iterators return same size and not missing or duplicated items during traversal
TEST_CASE("Iterator not missing/ duplicated elements"){
    MyContainer<int> c;
    for(int i: {1,2,3}){
        c.addElement(i);
    }    
    //Based about the fact that all iterators not change the data, they should return the same size as the container
    CHECK(to_vector<int>(c.begin_ascending_order(), c.end_ascending_order()).size()== c.size()); 
    CHECK(to_vector<int>(c.begin_descending_order(), c.end_descending_order()).size()== c.size());
    CHECK(to_vector<int>(c.begin_side_cross_order(), c.end_side_cross_order()).size()== c.size());
    CHECK(to_vector<int>(c.begin_reverse_order(), c.end_reverse_order()).size()== c.size());
    CHECK(to_vector<int>(c.begin_order(), c.end_order()).size()== c.size());
    CHECK(to_vector<int>(c.begin_middle_out_order(), c.end_middle_out_order()).size()== c.size());
}

//Cheack == and != on iterators for same container. Ensures that iterators can be compared correctly for loop
TEST_CASE("Iterator comparison operators"){
    MyContainer<int> c;
    for(int i: {1,2}){
        c.addElement(i);
    }    
    auto it1= c.begin_ascending_order(); //Point to the first element = 1
    auto it2= c.begin_ascending_order(); //Point to the first element = 1
    auto it_end= c.end_ascending_order(); //Point to the end of the container != 1
    CHECK(it1== it2);
    CHECK(it1!= it_end);
    ++it1; //Now it1 points to the second element = 2
    CHECK(it1!= it2); 
}

//Test to ensure iterators just for reading and not modifying the container according my implementation. The iterator gives a const reference
//so the value cannot be modified
TEST_CASE("Iterator returns const reference"){
    MyContainer<int> c;
    c.addElement(42);
    auto it= c.begin_ascending_order(); //AscendingOrder is used as an example 
    static_assert(std::is_same<decltype(*it), const int&>::value, "Should return const reference"); //Check the type of the dereferenced value is const int&
    //*it= 100; //Should not compile
    CHECK(*it == 42); //Just confirm it returns correct value
}

//Test iterator after container modification. Ensures old iterators are invalid, new ones are valid like I implemented in MyContainer.hpp
TEST_CASE("Old iterator invalid after container modification"){
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(2);
    auto old_it= c.begin_order();  //Create iterator before change
    c.addElement(3);  //Change container
    //Now the old iterator should throw on any operation
    CHECK_THROWS(*old_it); //Trying to used invalid iterator

    //Creating a new iterator should be fine like I planned in my implementation
    auto new_it= c.begin_order();
    vector<int> v= to_vector<int>(new_it, c.end_order()); //Using helper function to_vector to get the elements in order
    CHECK(v== vector<int>{1,2,3}); //Check if the new iterator returns the correct elements after modification like expected
}


//Ensures runtime exception when using invalid iterator by the comparing changes counter
TEST_CASE("Old iterator invalid after container modification"){
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    auto it = c.begin_ascending_order();
    c.addElement(30);
    CHECK_THROWS(*it);
}

// Reverse iterator: for even/odd sized containers
TEST_CASE("Reverse iterator on even and odd sized container"){
    //Even sized container
    MyContainer<int> even;
    even.addElement(1);
    even.addElement(2);
    even.addElement(3);
    even.addElement(4);
    CHECK(to_vector<int>(even.begin_reverse_order(), even.end_reverse_order())== vector<int>{4,3,2,1});
    //Odd sized container
    MyContainer<int> odd;
    odd.addElement(5);
    odd.addElement(6);
    odd.addElement(7);
    CHECK(to_vector<int>(odd.begin_reverse_order(), odd.end_reverse_order())== vector<int>{7,6,5});
}

//MiddleOut iterator with only one element
TEST_CASE("MiddleOut with single element") {
    MyContainer<int> c;
    c.addElement(99);
    CHECK(to_vector<int>(c.begin_middle_out_order(), c.end_middle_out_order())== vector<int>{99}); //Like another iterators, it should return the single element in the container
}

//Test ascending order with 1000 elements. Ensures that the iterator can handle large containers and returns elements in ascending order.
TEST_CASE("Ascending test on large container"){
    MyContainer<int> c;
    for(int i= 1000; i>= 1; --i){
        c.addElement(i); //Adding elements in reverse order to check if the iterator sorts them correctly
    }    
    auto result= to_vector<int>(c.begin_ascending_order(), c.end_ascending_order()); //Using helper function to_vector to get the elements in ascending order
    //Create expected vector with elements from 1 to 1000 in ascending order
    vector<int> expected(1000);
    for(int i= 0; i< 1000; ++i){
        expected[i]= i+ 1;
    }    
    CHECK(result== expected); //Check if the result matches the expected vectors
}
