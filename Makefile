CPP = g++
LINK = gcc

CPPFLAG = -Wall -ggdb -march=x86-64 -DDEBUG -I./include -I./depend/include
LINKFLAG = -lm -lstdc++ -lgtest -lpthread

CPPSRC = $(wildcard ./src/*.cpp)
CPPSRC += $(wildcard ./unit_test/*.cpp)
TESTCPPSRC = $(wildcard ./unit_test/*.cpp)

LIB_NAME = basis.a
TEST_NAME = basis_test

all : lib $(TEST_NAME)

lib : $(LIB_NAME)
$(LIB_NAME) : $(CPPSRC:.cpp=.o)
	ar rc -o $(LIB_NAME) $(CPPSRC:.cpp=.o)  $(LINKFLAG)
	@mv basis.a lib/basis.a

unit_test : $(TEST_NAME)
$(TEST_NAME) : $(TESTCPPSRC:.cpp=.o)
	$(LINK) -o $(TEST_NAME) $(TESTCPPSRC:.cpp=.o) $(LIB_NAME) $(LINKFLAG)

%.o : %.cpp
	$(CPP) -c $(CPPFLAG) $< -o $@

clean:
	@-rm -rf ./src/*.o ./unit_test/*.o $(TEST_NAME)