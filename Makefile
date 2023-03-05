CC=g++
CC1=clang++
FLAGS= -std=c++20 -Werror
LINK= -lfmt 
INCLUDE= -I./ -I./fmt/include
OBJS= parser_prot.o TableGen.o eval.o ActionGen.o
STACK= stack
LIB= libgram.a
all: $(LIB) $(STACK)
$(OBJS) : %.o : %.cc
	$(CC) -c $(FLAGS) $(INCLUDE) $^ -o $@
$(LIB): $(OBJS)
	ar rcs $@ $^
$(STACK): % : %.cpp
	$(CC) $(FLAGS) $(INCLUDE) $^ $(LIB)  -o $@
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(STACK) $(DESTDIR)$(PREFIX)/bin/$(STACK)
clean:
	rm -rf *.o $(STACK) *.a
