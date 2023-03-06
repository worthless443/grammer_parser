ifdef DEBUG
	DEBUG_GDB=-ggdb3
endif
CC=g++
CC1=clang++
FLAGS= -std=c++20 -Werror $(DEBUG_GDB)
LINK= -lfmt 
INCLUDE= -I./ -I./fmt/include -L./
OBJS= parser_prot.o TableGen.o eval.o ActionGen.o
STACK=stack.out
LIB= libgram.a
all: $(LIB) $(STACK)
$(OBJS) : %.o : %.cc
	$(CC) -c $(FLAGS) $(INCLUDE) $^ -o $@  
$(LIB): $(OBJS)
	ar rcs $@ $^
$(STACK): $(STACK:.out=.cpp)
	$(CC) $(FLAGS) $(INCLUDE) $^ $(LIB) -o $@
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(STACK) $(DESTDIR)$(PREFIX)/bin/$(STACK)
clean:
	rm -rf *.o $(STACK) *.a
