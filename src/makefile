CC=gcc
# CFLAGS=-I../include/ -O0 -g -pg
CFLAGS=-I../include/ -O2 
# LFLAGS=-O0 -g -pg
LFLAGS=-O2
DEPS=gc_shared.h gc_cheney.h gc_util.h gc_types.h gc_constants.h garbage_collector.h binary_tree.h cyclic_list.h cdouble_list.h test.h benchmark.h gc_cheney_base.h gc_custom.h tarray.h large_structure.h gc_generated.h
OBJ=gc_shared.o main.o gc_util.o gc_cheney.o gc_types.o garbage_collector.o binary_tree.o cyclic_list.o cdouble_list.o test.o benchmark.o gc_cheney_base.o gc_custom.o tarray.o large_structure.o gc_generated.o

%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
gc_benchmark: $(OBJ)
	$(CC) $(LFLAGS) -o $@ $^ $(CLFAGS)
	rm *.o
	mv $@ ../bin/$@