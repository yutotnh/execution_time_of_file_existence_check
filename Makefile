CC		= gcc
CFLAGS	= -O4 -Wall
OBJS	= main.o
PROGRAM	= main

all: $(PROGRAM)

$(PROGRAM):	$(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM)

clean:
	rm -f *.o *~ $(PROGRAM)
