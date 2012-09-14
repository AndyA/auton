.PHONY: all clean

VPATH = ../common

CFLAGS=-Wall -g3 -I../common
LDFLAGS=-lc -lm -lpthread

BINS=spi
OBJS=spi.o queue.o event.o util.o noticeboard.o
DEPS=$(OBJS:.o=.d) 

all: $(BINS)

spi: $(OBJS)

%.d: %.c
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

%.d: %.C
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

include $(DEPS)

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

clean:
	rm -f $(OBJS) $(DEPS) $(BINS) tags

