.PHONY: all clean

VPATH = ../lib

CFLAGS=-Wall -ggdb -I../lib -D_DEBUG
CXXFLAGS=-Wall -ggdb -I../lib -D_DEBUG
LDFLAGS=-lc -lm -lstdc++

BIN=sim
SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o) brain.o instinct.o observation.o imath.o
DEPS=$(OBJS:.o=.d) 

all: $(BIN)

$(BIN): $(OBJS)

%.d: %.c
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

%.d: %.cpp
	@$(SHELL) -ec '$(CC) -MM $(CXXFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

include $(DEPS)

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

clean:
	rm -f *.o $(OBJS) $(DEPS) $(BIN) tags

