#vanunuraz@gmail.com
#Compiler and flags:
#g++ is the C++ compiler
#-std=c++20 use the C++20 standard
#-Wall for warnings
#-g for debug information for tools valgrind
CXX= g++
CXXFLAGS= -std=c++20 -Wall -g

#Source and header files
SRC= main.cpp #main program file
TEST= test.cpp #test file using doctest
HEADER= MyContainer.hpp #header file with container and iterators

#Names for the output executables
MAIN_EXEC= main #for the main program
TEST_EXEC= tests #for tests

#Define o files from .cpp files
MAIN_OBJ= $(SRC:.cpp=.o)
TEST_OBJ= $(TEST:.cpp=.o)

#Phony targets are not files
.PHONY: all Main test valgrind clean

#Default target: build and run main and tests
all: Main test

#Build and run the main
Main: $(MAIN_EXEC)
	@echo "Running main executable..."
	./$(MAIN_EXEC)

#Link object files to create the main executable
$(MAIN_EXEC): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

#Build and run tests
test: $(TEST_EXEC)
	@echo "Running test executable..."
	./$(TEST_EXEC)

#Link object files to create the test executable
$(TEST_EXEC): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

#Compile .cpp files to .o files
%.o: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#Run valgrind to check memory leaks in the main program
valgrind: $(MAIN_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(MAIN_EXEC)

#Clean up: remove all object files and executables
clean:
	rm -f *.o $(MAIN_EXEC) $(TEST_EXEC)
