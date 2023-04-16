CC=g++
CC1=clang++
FLAGS= -std=c++20 -Werror
LINK= -lfmt
INCLUDE= -I./ -I./fmt/include -L./fmt/build -L./
OBJS=parser_prot.o TableGen.o eval.o ActionGen.o stack.o
MAIN=main.cc
LIB_LINK=-lfmt -lgram
LIB=libgram.a
ifdef THREAD
FLAGS+=-DTHREAD=$(THREAD)
else
FLAGS+=-DTHREAD=16
endif

all: $(LIB) parser
$(OBJS) : %.o : %.cc
	$(CC) -c $(FLAGS) $(INCLUDE) $^ -o $@
$(LIB): $(OBJS)
	ar rcs $@ $^
parser: $(MAIN)
	$(CC) $(FLAGS) $(INCLUDE) $^ $(LIB_LINK)  -o $@
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(STACK) $(DESTDIR)$(PREFIX)/bin/$(STACK)
clean:
	rm -rf *.o parser *.a
