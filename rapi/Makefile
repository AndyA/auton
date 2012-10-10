.PHONY: all clean

VPATH = ../common

CFLAGS=-Wall -g3 -I../common
LDFLAGS=-lc -lm -lpthread -lasound

BINS=spi sounder
BINOBJS=$(addsuffix .o,$(BINS))
MISCOBJS=util.o
SPIOBJS=queue.o event.o noticeboard.o
OBJS=$(BINOBJS) $(MISCOBJS) $(SPIOBJS)
DEPS=$(OBJS:.o=.d) 

all: $(BINS)

spi: $(SPIOBJS)

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
	rm -f $(OBJS) $(DEPS) $(BINS) tags
