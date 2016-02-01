CPP = g++
LINK = gcc
AR = ar rcs

CPPFLAG = -Wall -ggdb -march=x86-64 -DDEBUG -I./include -I./depend/include
LINKFLAG = -lm -lstdc++ -lgtest -lpthread

CPPSRC = $(wildcard ./src/*.cpp)
TESTCPPSRC = $(wildcard ./unit_test/*.cpp)

LIB_DIR = lib
LIB_NAME = libbasis.a
TEST_NAME = basis_test

all : $(LIB_NAME) $(TEST_NAME)

lib : $(LIB_NAME)
$(LIB_NAME) : $(CPPSRC:.cpp=.o)
	$(AR) $(LIB_NAME) $(CPPSRC:.cpp=.o)
	@mkdir -p $(LIB_DIR)
	@mv $(LIB_NAME) lib/

unit_test : $(TEST_NAME)
$(TEST_NAME) : $(TESTCPPSRC:.cpp=.o)
	$(LINK) -o $(TEST_NAME) $(TESTCPPSRC:.cpp=.o) $(LIB_DIR)/$(LIB_NAME) $(LINKFLAG)

%.o : %.cpp
	$(CPP) -c $(CPPFLAG) $< -o $@

clean:
	@-rm -rf ./src/*.o ./unit_test/*.o $(TEST_NAME)
