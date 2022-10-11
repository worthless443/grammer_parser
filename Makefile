CC=g++
CC1=clang++
FLAGS=-std=c++20 -lfmt   -Werror
INCLUDE=-I./
OBJS=parser_prot.o TableGen.o eval.o ActionGen.o
STACK=stack
LIB=libgram.a
all: $(LIB) $(STACK)
$(OBJS) : %.o : %.cc
	$(CC) -c $(FLAGS) $(INCLUDE) $^ -o $@
$(LIB): $(OBJS)
	ar rcs $@ $^
$(STACK): % : %.cpp
	$(CC) $(FLAGS) $(INCLUDE) $^ $(LIB)  -o $@
install:
	cp $(STACK) /usr/bin/stack
clean:
	rm -rf *.o $(STACK)
