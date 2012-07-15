VPATH=..

CPPFLAGS=-DTEST -g3 -std=gnu99 -Wall \
	 -Wstrict-prototypes \
	 -funsigned-char -funsigned-bitfields -fpack-struct

TESTS=00mavg.t 00mavg_zero.t
OBJS=avr_env.o mavg.o tap.o

all: $(TESTS)

mavg.o: ../mavg.c ../mavg.h

%.t: %.o $(OBJS)
	$(CC) -lm -lc -o $@ $(OBJS) $<

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(TESTS) $(TESTS:.t=.o) $(TESTS:.t=.d)

%.d: %.c
	@$(SHELL) -ec '$(CC) -MM $(CPPFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

include $(OBJS:.o=.d) 

test: all
	prove -e '' $(addprefix ./, $(TESTS))

.PHONY: test
