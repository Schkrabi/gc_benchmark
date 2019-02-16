#CC=gcc
CC=gcc
#CFLAGS=-I../include/ -I./myjit/ -O0 -g -pg
CFLAGS=-I../include/ -I./myjit/ -O2
#LFLAGS=-O0 -g -pg
LFLAGS=-O2
JITFLAGS=-c -g -Winline -Wall -std=c99 -pedantic -D_XOPEN_SOURCE=600
DEPS=gc_shared.h gc_cheney.h gc_util.h gc_types.h gc_constants.h garbage_collector.h binary_tree.h cyclic_list.h cdouble_list.h test.h benchmark.h gc_cheney_base.h gc_custom.h tarray.h large_structure.h gc_generated.h graph.h generated_types.h
OBJ=gc_shared.o main.o gc_util.o gc_cheney.o gc_types.o garbage_collector.o binary_tree.o cyclic_list.o cdouble_list.o test.o benchmark.o gc_cheney_base.o gc_custom.o tarray.o large_structure.o gc_generated.o graph.o generated_types.o

all: clean gc_benchmark_100_binary_trees gc_benchmark_200_binary_trees gc_benchmark_300_binary_trees gc_benchmark_400_binary_trees gc_benchmark_500_binary_trees gc_benchmark_600_binary_trees gc_benchmark_700_binary_trees gc_benchmark_800_binary_trees gc_benchmark_900_binary_trees gc_benchmark_1000_binary_trees gc_benchmark_150_graphs gc_benchmark_180_graphs gc_benchmark_210_graphs gc_benchmark_240_graphs gc_benchmark_270_graphs gc_benchmark_300_graphs gc_benchmark_450_graphs gc_benchmark_600_graphs gc_benchmark_750_graphs gc_benchmark_900_graphs gc_benchmark_1050_graphs gc_benchmark_1200_graphs gc_benchmark_full gc_benchmark_none

%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
gc_benchmark: $(OBJ) jitlib-core.o
	$(CC) $(LFLAGS) -o $@ $^ $(CLFAGS)
	mv $@ ../bin/$@

gc_benchmark_100_binary_trees:
	rm -f *.o
	cp ../include/generated_types/generated_types_100_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_200_binary_trees:	
	rm -f *.o
	cp ../include/generated_types/generated_types_200_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_300_binary_trees:	
	rm -f *.o
	cp ../include/generated_types/generated_types_300_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_400_binary_trees:	
	rm -f *.o
	cp ../include/generated_types/generated_types_400_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_500_binary_trees:	
	rm -f *.o
	cp ../include/generated_types/generated_types_500_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_600_binary_trees:	
	rm -f *.o
	cp ../include/generated_types/generated_types_600_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_700_binary_trees:
	rm -f *.o
	cp ../include/generated_types/generated_types_700_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_800_binary_trees:
	rm -f *.o
	cp ../include/generated_types/generated_types_800_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_900_binary_trees:	
	rm -f *.o
	cp ../include/generated_types/generated_types_900_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_1000_binary_trees:	
	rm -f *.o
	cp ../include/generated_types/generated_types_1000_binary_trees.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_150_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_150_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@
	
gc_benchmark_180_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_180_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@
	
gc_benchmark_210_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_210_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_240_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_240_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@
	
gc_benchmark_270_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_270_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_300_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_300_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_450_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_450_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@
			
gc_benchmark_600_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_600_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_750_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_750_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_900_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_900_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_1050_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_1050_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_1200_graphs:
	rm -f *.o
	cp ../include/generated_types/generated_types_1200_graphs.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_full:
	rm -f *.o
	cp ../include/generated_types/generated_types_full.h ../include/generated_types.h
	make gc_benchmark
	mv ../bin/gc_benchmark ../bin/$@

gc_benchmark_none:
	rm -f *.o
	cp ../include/generated_types/generated_types_none.h ../include/generated_types.h
	make gc_benchmark

jitlib-core.o: myjit/jitlib.h myjit/jitlib-core.h myjit/jitlib-core.c myjit/jitlib-debug.c myjit/x86-codegen.h myjit/x86-specific.h myjit/reg-allocator.h myjit/flow-analysis.h myjit/set.h myjit/amd64-specific.h myjit/amd64-codegen.h myjit/sparc-codegen.h myjit/sparc-specific.h myjit/llrb.c myjit/reg-allocator.h myjit/rmap.h myjit/cpu-detect.h myjit/x86-common-stuff.c myjit/common86-specific.h myjit/common86-codegen.h myjit/sse2-specific.h myjit/code-check.c
	$(CC) -c -g -Winline -Wall -std=c99 -pedantic -D_XOPEN_SOURCE=600 myjit/jitlib-core.c
	
clean:
	rm -f *.o
	rm -f disasm/myjit-disasm
	rm -f ../bin/*
