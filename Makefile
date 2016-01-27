CPP = g++
LINK = gcc

CPPFLAG = -Wall -ggdb -march=x86-64 -DDEBUG -I./include
LINKFLAG = -lm -lstdc++ -lpthread

CPPSRC = $(wildcard ./src/*.cpp)

OBJECT_NAME = basis_test

all : $(OBJECT_NAME)

$(OBJECT_NAME): $(CPPSRC:.cpp=.o)
	$(LINK) -o $(OBJECT_NAME) $(CPPSRC:.cpp=.o)  $(LINKFLAG)

%.o : %.cpp
	$(CPP) -c $(CPPFLAG) $< -o $@

clean:
	-rm -rf ./src/*.o $(OBJECT_NAME)