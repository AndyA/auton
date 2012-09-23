.PHONY: all clean

VPATH = ../lib

CFLAGS=-Wall -g3 -I../lib
LDFLAGS=-lc -lm -lstdc++

SRC=$(wildcard *.t)
TESTS=$(SRC:.t=.T)
COMOBJ=tap.o
TSTOBJ=$(SRC:.t=.o)
OBJS=$(COMOBJ) $(TSTOBJ)
DEPS=$(OBJS:.o=.d) 

test: $(TESTS)
	prove -e '' $(addprefix ./,$(TESTS))

10_imath.T: imath.o
20_observation.T: observation.o imath.o
30_brain.T: brain.o instinct.o observation.o imath.o

%.T: %.o $(COMOBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -x c++ -c -o $@ $<

%.o: %.t
	$(CC) $(CPPFLAGS) $(CFLAGS) -x c++ -c -o $@ $<

%.d: %.t
	@$(SHELL) -ec '$(CC) -x c++ -MM $(CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

%.d: %.cpp
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

%.d: %.c
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

include $(DEPS)

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

clean:
	rm -f *.o $(OBJS) $(DEPS) $(TESTS) tags

