GTEST_DIR=~/googletest/googletest
PROJ=linked_list

TEST_CASE=$(PROJ)_testcase
TEST_CASE_EX=$(PROJ)_testcase_ex

COPTS=-fprofile-arcs -ftest-coverage
LDFLAGS=-fprofile-arcs -ftest-coverage

default: test

main: main.o $(PROJ).o
	gcc main.o $(PROJ).o -o main

test: Gtest_main.o $(TEST_CASE).o $(TEST_CASE_EX).o libgtest.a $(PROJ).o
	g++ -I $(GTEST_DIR)/include -pthread $(TEST_CASE).o $(TEST_CASE_EX).o $(PROJ).o Gtest_main.o libgtest.a $(COPTS) -o my_test

######## Googletest Lib ##########
libgtest.a:
	g++ -isystem $(GTEST_DIR)/include -I $(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

##### Test ###############
Gtest_main.o: Gtest_main.cpp
	g++ -c -isystem $(GTEST_DIR)/include $(COPTS) -I $(GTEST_DIR) Gtest_main.cpp

$(TEST_CASE).o: $(TEST_CASE).cpp
	g++ -c -isystem $(GTEST_DIR)/include -I $(GTEST_DIR)  $(TEST_CASE).cpp

$(TEST_CASE_EX).o: $(TEST_CASE_EX).cpp
	g++ -c -isystem $(GTEST_DIR)/include -I $(GTEST_DIR)  $(TEST_CASE_EX).cpp

##### Normal #######################
$(PROJ).o: $(PROJ).c $(PROJ).h
	g++ -c $(PROJ).c

main.o: main.c $(PROJ).h
	gcc -c main.c

###########################
clean:
	rm -f *.o
	rm -f *.gcno

report:
	COV_OUTPUT=./cov_output
	lcov -c -i -d . -o .coverage.base
	lcov -c -d . -o .coverage.run
	lcov -d . -a .coverage.base -a .coverage.run -o .coverage.total
	genhtml --no-branch-coverage -o $(COV_OUTPUT) .coverage.total
	rm -f .coverage.base .coverage.run .coverage.tota
