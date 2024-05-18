ifdef DEBUG
	DEBUG_GDB=-ggdb3
endif
CC=g++
CC1=clang++
FLAGS= -std=c++20 -Werror  -ggdb
LINK= -lfmt
INCLUDE= -I./include -I./fmt/include -L./fmt/build -L./src
OBJS=src/parser_prot.o src/TableGen.o src/eval.o src/ActionGen.o src/stack.o
MAIN=src/main.cc
LIB_LINK=-lfmt -lgram
LIB=src/libgram.a
FMT=fmt/build/libfmt.a
ifdef THREAD
FLAGS+=-DTHREAD=$(THREAD)
else
FLAGS+=-DTHREAD=16
endif

all: $(FMT) $(LIB) src/main
$(FMT):
	cmake fmt -B fmt/build
	make -C fmt/build -j16
$(OBJS) : %.o : %.cc
	$(CC) -c $(FLAGS) $(INCLUDE) $^ -o $@  
$(LIB): $(OBJS)
	ar rcs $@ $^
src/main: $(MAIN) $(LIB)
	$(CC) $(FLAGS) $(INCLUDE) $^ $(LIB_LINK)  -o $(patsubst %.cc,%,$(MAIN))
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(STACK) $(DESTDIR)$(PREFIX)/bin/$(STACK)
clean:
	rm -rf src/*.o parser src/*.a
