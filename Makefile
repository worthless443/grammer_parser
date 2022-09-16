CC=g++
FLAGS=-std=c++20 -lfmt 
INCLUDE=-I./
OBJS=parser_prot.o TableGen.o eval.o
STACK=stack
all: $(OBJS) $(STACK)
$(OBJS) : %.o : %.cc
	$(CC) -c $(FLAGS) $(INCLUDE) $^ -o $@

$(STACK): % : %.cpp
	$(CC) $(FLAGS) $(INCLUDE) $^ $(OBJS)  -o $@
clean:
	rm -rf *.o $(STACK)
