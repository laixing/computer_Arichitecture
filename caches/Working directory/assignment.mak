CFLAGS = -O
CC = g++

linear-chase: linear-chase.o chase-pointers.o
	$(CC) $(CFLAGS) -Wall -o linear-chase linear-chase.o chase-pointers.o
	./linear-chase
random-chase: random-chase.o chase-pointers.o
	$(CC) $(CFLAGS) -Wall -o random-chase random-chase.o chase-pointers.o
	./random-chase
	
linear-chase.o: linear-chase.cpp
	$(CC) $(CFLAGS) -Wall -c linear-chase.cpp
chase-pointers.o: chase-pointers.cpp
	$(CC) $(CFLAGS) -Wall -c chase-pointers.cpp
random-chase.o: random-chase.cpp
	$(CC) $(CFLAGS) -Wall -c random-chase.cpp

clean:
	rm -f core *.o 
