CC=gcc
CFLAGS=-I../include/ -g
DEPS=gc_shared.h gc_cheney.h gc_util.h garbage_collector.h binary_tree.h cyclic_list.h
OBJ=gc_shared.o main.o gc_util.o gc_cheney.o garbage_collector.o binary_tree.o cyclic_list.o

%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
gc_benchmark: $(OBJ)
	$(CC) -o $@ $^ $(CLFAGS)
	rm *.o
	mv $@ ../bin/$@