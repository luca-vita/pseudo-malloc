CC=gcc
CCOPTS=--std=gnu99 -Wall -Wextra
AR=ar
RM=rm

OBJS=bitmap.o\
     buddy_allocator.o\
	 my_allocator.o

HEADERS=bitmap.h\
        buddy_allocator.h\
		my_allocator.h

LIBS=libbuddy.a

BINS=test

.PHONY: clean all

all: $(LIBS) $(BINS)

%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<

libbuddy.a: $(OBJS)
	$(AR) -rcs $@ $^
	$(RM) $(OBJS)

test: main.o $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ -lm

clean:
	$(RM) -rf *.o *~ $(LIBS) $(BINS)