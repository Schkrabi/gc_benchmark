CC=gcc
CFLAGS=-I../include/ -g -pg -O3
DEPS=gc_shared.h gc_cheney.h gc_util.h gc_types.h gc_constants.h garbage_collector.h binary_tree.h cyclic_list.h cdouble_list.h test.h benchmark.h gc_cheney_base.h gc_custom.h entanglement.h
OBJ=gc_shared.o main.o gc_util.o gc_cheney.o gc_types.o garbage_collector.o binary_tree.o cyclic_list.o cdouble_list.o test.o benchmark.o gc_cheney_base.o gc_custom.o entanglement.o

%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
gc_benchmark: $(OBJ)
	$(CC) -pg -o $@ $^ $(CLFAGS)
	rm *.o
	mv $@ ../bin/$@