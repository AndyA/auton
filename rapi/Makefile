.PHONY: all clean

VPATH = ../common

CFLAGS=-Wall -g3 -I../common
LDFLAGS=-lc -lm -lpthread -lasound

BINS=spi sounder
BINOBJS=$(addsuffix .o,$(BINS))
MISCOBJS=util.o
SPIOBJS=queue.o event.o noticeboard.o
SOUNDEROBJS=eventio.o synth.o sin.o itrig.o
OBJS=$(BINOBJS) $(MISCOBJS) $(SPIOBJS) $(SOUNDEROBJS)
DEPS=$(OBJS:.o=.d) 
GENERATED=sin.c

all: $(BINS)

spi: $(SPIOBJS)

sounder: $(SOUNDEROBJS)

sin.c: tools/mksin.pl
	perl tools/mksin.pl > sin.c

%: %.o $(MISCOBJS)
	cc -o $@ $^ $(LDFLAGS)

%.d: %.c
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

include $(DEPS)

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

clean:
	rm -f $(OBJS) $(DEPS) $(BINS) $(GENERATED) tags
