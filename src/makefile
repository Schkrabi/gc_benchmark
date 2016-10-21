CC=gcc
CFLAGS=-I../include/ -g
DEPS=gc_shared.h
OBJ=gc_shared.o main.o gc_util.o gc_cheney.o

%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
gc_benchmark: $(OBJ)
	$(CC) -o $@ $^ $(CLFAGS)
	rm *.o
	mv $@ ../bin/$@