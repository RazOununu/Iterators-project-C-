//vanunuraz@gmail.com
//This header defines a templated container class "MyContainer" that accepts only int, double, or string types. This container supports various operations
//like adding/removing elements. In this container also implements iterators that allow traversal of the container in different orders.

//The base iterator class "IteratorBase" provides common functionality for all iterators, including dereferencing, incrementing, and comparison operators. 
//Six different iterator types at "MyContainer" inherits from a base iterator class, each implementing a specific order of traversal: ascending, descending, reverse, order, sideCross, and middleOut.
//I decided implement a mechanism to check if the container has changed since the iterator was created, throwing an exception if it has.

#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
using namespace std;

namespace exercise4{

//IteratorBase: Shared base class for all iterators by template. Holds a local copy of the data, tracks the current index, and checks if the container has changed.
//Ensures safety when accessing data by throwing an exception if the container was modified.
    template<typename T>
    class IteratorBase{
        protected:
            vector<T> data; //The data in vector at the time of iterator creation
            size_t index; //Current index in the iteration
            const int* currentChanges; //Pointer to the change counter in MyContainer
            int changesAtCreateIter; //The value of the change counter when this iterator was created

            //Check if container has changed since iterator was created
            void compareChanges() const{
                if(*currentChanges!= changesAtCreateIter){
                    throw runtime_error("Iterator invalid because the container was modified");
                }
            }

        public:
            //Before using each action, call compareChanges to ensure the iterator is still valid

            //This operator returns reference to the current element in the iteration.
            const T& operator*() const{
                compareChanges();
                return data.at(index); //Operator for accessing the current element at index. at return error if index is out of the range.
            }

            //This operator returns a pointer to the current element in the iteration that allows access to its members.
            const T* operator->() const{
                compareChanges();
                return &data.at(index); //Pointer access operator for the current element
            }

            //Pre-increment
            IteratorBase& operator++(){
                compareChanges();
                ++index; 
                return *this;
            }

            //Post-increment
            IteratorBase operator++(int){
                compareChanges();
                IteratorBase tmp = *this; //Create a copy of the current state. The copy is automatically by compiler.
                ++(*this);
                return tmp;
            }

            //This operator checks if the current iterator position not equal to the end position. It is used to determine if the iteration should continue.
            bool operator!=(const IteratorBase& other) const{
                compareChanges();
                return index != other.index; //Inequality check
            }

            //This operator checks if the current iterator position equal to the other iterator position, and also checks if the data is the same. It is used for
            //testing equality between two iterators.
            bool operator==(const IteratorBase& other) const{
                compareChanges();
                return index == other.index && data == other.data; //Equality check
            }
    };

    //MyContainer: A generic container for int, double, or string. Includes methods to add/remove elements and iterators for various traversal orders that
    //inherit from IteratorBase.

    template <typename T=int>//Default type is int, but can be specialized for double or string, like we asked.
    class MyContainer{
        //Using static_assert for compile-time type checking to ensure T is one of the allowed types.
        static_assert(
            std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, std::string>,
            "MyContainer supports only int, double, or string"
        );

        private:
            vector<T> data; //Data storage in a vector. Using vector for dynamic array-like behavior, allowing easy addition/removal of elements.
            int changes = 0; //Changes counter for iterator validation

        public:
            MyContainer() = default; //Default constructor for creating an empty container. In the iterators implemented a constructor I takes a
            //MyContainer object and initializes the iterator with its data.

            //Add a new element and increment the change counter
            void addElement(const T& element){
                data.push_back(element);
                changes++; //Now the iterator not valid for another action because the container has changed.
            }

            //Remove all occurrences of the given element, or throw if not found
            void remove(const T& element){
                auto it= std::remove(data.begin(), data.end(), element); //This function removes all occurrences of the element from the vector and returns
                //an iterator to the new end of the vector.
                if(it == data.end()){
                    throw invalid_argument("Element not found in the container");
                }
                data.erase(it, data.end()); //This function erases the elements from the vector that were removed by std::remove.
                ++changes; //Now the iterator not valid for another action because the container has changed.
            }

            //Return number of elements in the container
            size_t size() const{
                return data.size();
            }

            //Output container contents in [ , , ...] format
            //Friend function to allow access to private members for printing, the operator<< is overloaded to print the container elements.
            friend ostream& operator<<(ostream& os, const MyContainer<T>& container){
                os<< "[";
                for(size_t i= 0; i< container.data.size(); ++i){
                    os<< container.data[i];
                    if (i!= container.data.size()- 1){ //If not the last element, add a ,
                        os<< ", ";
                    }
                }
                os<< "]"; //Its the end of the output format
                return os;
            }

            //Helper getters for iterators implementation
            //Get the data:2. For copy of the data at the time of iterator creation.
            vector<T> getElements() const{
            return data;
            }
            //Get the changes counter (at the time of creation of the iterator):
            int getChanges() const{
                return changes;
            }
            //Get a pointer to the changes counter, used by iterators to check if the container has changed since iterator creation.
            const int* getChangesPointer() const{
                return &changes;
            }

        //Iterators in this container class: each iterator has its own order logic and inherits from IteratorBase the overloaded operators.
        //In this part of the code I implement constructors for each iterator type.

        class AscendingOrder: public IteratorBase<T>{
            public:
                AscendingOrder(const MyContainer<T>& container, bool end= false){
                    this->data= container.getElements(); 
                    sort(this->data.begin(), this->data.end()); //Using std::sort to sort the data in ascending order
                    this->index= end? this->data.size(): 0; //If end is true, set index to the size of data, otherwise set it to 0
                    //Set the current changes pointer and the changes at the time of iterator creation for comparing later
                    this->currentChanges= container.getChangesPointer(); 
                    this->changesAtCreateIter= container.getChanges(); 
                }
        };

        class DescendingOrder: public IteratorBase<T>{
            public:
                DescendingOrder(const MyContainer<T>& container, bool end= false){
                    this->data= container.getElements();
                    sort(this->data.begin(), this->data.end(), greater<T>());//Using std::sort with greater<T>() to sort the data in descending order
                    this->index= end? this->data.size(): 0; //If end is true, set index to the size of data, otherwise set it to 0
                    //Set the current changes pointer and the changes at the time of iterator creation for comparing later
                    this->currentChanges= container.getChangesPointer();
                    this->changesAtCreateIter= container.getChanges();
                }
            };

        class ReverseOrder: public IteratorBase<T>{
            public:
                ReverseOrder(const MyContainer<T>& container, bool end= false){
                    this->data= container.getElements();
                    reverse(this->data.begin(), this->data.end()); //Using std::reverse to reverse the order of elements in the vector
                    this->index= end? this->data.size(): 0; //If end is true, set index to the size of data, otherwise set it to 0
                    //Set the current changes pointer and the changes at the time of iterator creation for comparing later
                    this->currentChanges= container.getChangesPointer();
                    this->changesAtCreateIter= container.getChanges();
                }
        };

        class Order: public IteratorBase<T>{
            public:
                //This iterator just iterates over the elements in the order they were added
                Order(const MyContainer<T>& container, bool end= false){
                    this->data= container.getElements();
                    this->index= end? this->data.size(): 0; //If end is true, set index to the size of data, otherwise set it to 0
                    //Set the current changes pointer and the changes at the time of iterator creation for comparing later
                    this->currentChanges= container.getChangesPointer();
                    this->changesAtCreateIter= container.getChanges();
                }
        };

        class SideCrossOrder: public IteratorBase<T>{
            public:
                SideCrossOrder(const MyContainer<T>& container, bool end= false){
                    vector<T> sorted= container.getElements(); //Create a copy of the elements
                    sort(sorted.begin(), sorted.end()); //Sort the elements in ascending order
                    this->data.clear(); //Clear the data from the original vector

                    //point to the beginning and end of the container
                    int left= 0;
                    int right= sorted.size()- 1;
                    while(left <= right){ //While the left index is less than or equal to the right index
                        this->data.push_back(sorted[left++]); //Add the leftmost element
                        if(left <= right){
                            this->data.push_back(sorted[right--]);//Add the rightmost element
                        }
                    }
                    this->index= end? this->data.size(): 0; //If end is true, set index to the size of data, otherwise set it to 0
                    //Set the current changes pointer and the changes at the time of iterator creation for comparing later
                    this->currentChanges= container.getChangesPointer();
                    this->changesAtCreateIter= container.getChanges();
                }
        };

        //To handle with memory leak, I use size_t because .size() returns size_t= unsigned long and its problem to compare with int.
        class MiddleOutOrder: public IteratorBase<T>{
            public:
                MiddleOutOrder(const MyContainer<T>& container, bool end= false){
                //Create a copy of the elements and sort them
                vector<T> sorted= container.getElements();
                sort(sorted.begin(), sorted.end());
                this->data.clear(); //Clear the data from the original vector

                size_t size= sorted.size(); //Get the size of the sorted vector
                //If the size is 0, set the index to 0
                if(size== 0){
                    this->index = 0;
                }
                //If the size is odd, start from the middle and alternate between left and right elements
                if(size%2== 1){
                    size_t mid= size/ 2;
                    this->data.push_back(sorted[mid]);
                    size_t left= mid- 1;
                    size_t right= mid+ 1;
                    bool leftTurn= true;
                    //Alternating between left and right elements until all elements are added
                    while(left< size || right< size){
                        //Left
                        if(leftTurn&& left< size){
                            this->data.push_back(sorted[left--]);
                        }
                        //Right
                        if(!leftTurn && right< size){
                            this->data.push_back(sorted[right++]);
                        }
                        leftTurn= !leftTurn; //Switch between left and right
                    }
                }
                //If the size is even, start from the left of center and alternate between left and right elements
                else{
                    //Mid_left is the left middle element and mid_right is the right middle element
                    size_t mid_left= size/ 2-1;
                    size_t mid_right= size/ 2; 
                    this->data.push_back(sorted[mid_left]);

                    size_t left= (mid_left> 0)? mid_left- 1: size; //If mid_left is 0, set left to size to avoid out of range
                    size_t right= mid_right; //Start from the right middle element at the right side
                    bool rightTurn= true;
                    //Start from the right of center and alternate between left and right elements until all elements are added
                    while(left< size || right< size){
                        if(rightTurn && right< size){
                            this->data.push_back(sorted[right++]);
                        }
                        else if(!rightTurn && left< size){
                            this->data.push_back(sorted[left--]);
                        }
                        rightTurn= !rightTurn; //Switch between left and rights
                    }
                }
                    this->index= end? this->data.size(): 0; //If end is true, set index to the size of data, otherwise set it to 0
                    //Set the current changes pointer and the changes at the time of iterator creation for comparing later
                    this->currentChanges= container.getChangesPointer();
                    this->changesAtCreateIter= container.getChanges();
                }
        };

        //Iterator Accessors: each function creates and returns a begin/ end iterator of a specific order. Used for iterating over the container in different orders.
        //After the implementation of the iterators, I implement the begin and end functions for each iterator type because they are used to create the iterators.
        AscendingOrder begin_ascending_order() const{
            return AscendingOrder(*this, false); //Create AscendingOrder iterator with *this as the container and false to indicate the beginning of the iteration
        }
        AscendingOrder end_ascending_order() const{
            return AscendingOrder(*this, true); //Create AscendingOrder iterator with *this as the container and true to indicate the end of the iteration
        }

        DescendingOrder begin_descending_order() const{
            return DescendingOrder(*this, false); //Create DescendingOrder iterator with *this as the container and false to indicate the beginning of the iteration
        }
        DescendingOrder end_descending_order() const{
            return DescendingOrder(*this, true); //Create DescendingOrder iterator with *this as the container and true to indicate the end of the iteration
        }

        ReverseOrder begin_reverse_order() const{
            return ReverseOrder(*this, false); //Create ReverseOrder iterator with *this as the container and false to indicate the beginning of the iteration
        }
        ReverseOrder end_reverse_order() const{
            return ReverseOrder(*this, true); //Create ReverseOrder iterator with *this as the container and true to indicate the end of the iteration
        }

        Order begin_order() const{
            return Order(*this, false); //Create Order iterator with *this as the container and false to indicate the beginning of the iteration
        }
        Order end_order() const{
            return Order(*this, true); //Create Order iterator with *this as the container and true to indicate the end of the iteration
        }

        SideCrossOrder begin_side_cross_order() const{
            return SideCrossOrder(*this, false); //Create SideCrossOrder iterator with *this as the container and false to indicate the beginning of the iteration
        }
        SideCrossOrder end_side_cross_order() const{
            return SideCrossOrder(*this, true); //Create SideCrossOrder iterator with *this as the container and true to indicate the end of the iteration
        }

        MiddleOutOrder begin_middle_out_order() const{
            return MiddleOutOrder(*this, false); //Create MiddleOutOrder iterator with *this as the container and false to indicate the beginning of the iteration
        }
        MiddleOutOrder end_middle_out_order() const{
            return MiddleOutOrder(*this, true); //Create MiddleOutOrder iterator with *this as the container and true to indicate the end of the iteration
        }
    }; //End of MyContainer class
} //End of namespace exercise4
