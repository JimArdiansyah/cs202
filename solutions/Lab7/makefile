CFLAGS ?= -std=c++11 -Wall -Wextra -Iinclude

# -------------------------
# This means that when you just type "make", you make all of the executables

ALL = bin/cp_tester

all: $(ALL)

# -------------------------
# "make develop" makes the program

DEVELOP = bin/cp_tester 

develop: $(DEVELOP)

# -------------------------
# "make clean" deletes the object files and binaries

clean:
	rm -f obj/* bin/*

# -------------------------
# Object files

obj/code_processor.o: src/code_processor.cpp include/code_processor.hpp
	g++ $(CFLAGS) -c -o obj/code_processor.o src/code_processor.cpp

obj/cp_tester.o: src/cp_tester.cpp include/code_processor.hpp
	g++ $(CFLAGS) -c -o obj/cp_tester.o src/cp_tester.cpp	

# Add a rule to compile double_checker.cpp
obj/double_checker.o: src/double_checker.cpp
	g++ $(CFLAGS) -c -o obj/double_checker.o src/double_checker.cpp	

# Add a rule to compile random_codes.cpp
#obj/random_codes.o: src/random_codes.cpp
#	g++ $(CFLAGS) -c -o obj/random_codes.o src/random_codes.cpp

# -------------------------
# Executables

bin/cp_tester: obj/cp_tester.o obj/code_processor.o obj/double_checker.o 
	g++ $(CFLAGS) -o bin/cp_tester obj/cp_tester.o obj/code_processor.o obj/double_checker.o 