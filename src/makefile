CC=gcc
CFLAGS=-I../include/ -I./myjit/ -O0 -g -pg
#CFLAGS=-I../include/ -O2 
LFLAGS=-O0 -g -pg
#LFLAGS=-O2
JITFLAGS=-c -g -Winline -Wall -std=c99 -pedantic -D_XOPEN_SOURCE=600
DEPS=gc_shared.h gc_cheney.h gc_util.h gc_types.h gc_constants.h garbage_collector.h binary_tree.h cyclic_list.h cdouble_list.h test.h benchmark.h gc_cheney_base.h gc_custom.h tarray.h large_structure.h gc_generated.h
OBJ=gc_shared.o main.o gc_util.o gc_cheney.o gc_types.o garbage_collector.o binary_tree.o cyclic_list.o cdouble_list.o test.o benchmark.o gc_cheney_base.o gc_custom.o tarray.o large_structure.o gc_generated.o

all: gc_benchmark

recompile: clean gc_benchmark

%.o: ./src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
gc_benchmark: $(OBJ) jitlib-core.o
	$(CC) $(LFLAGS) -o $@ $^ $(CLFAGS)
	mv $@ ../bin/$@
	
jitlib-core.o: myjit/jitlib.h myjit/jitlib-core.h myjit/jitlib-core.c myjit/jitlib-debug.c myjit/x86-codegen.h myjit/x86-specific.h myjit/reg-allocator.h myjit/flow-analysis.h myjit/set.h myjit/amd64-specific.h myjit/amd64-codegen.h myjit/sparc-codegen.h myjit/sparc-specific.h myjit/llrb.c myjit/reg-allocator.h myjit/rmap.h myjit/cpu-detect.h myjit/x86-common-stuff.c myjit/common86-specific.h myjit/common86-codegen.h myjit/sse2-specific.h myjit/code-check.c myjit/arm32-codegen.h myjit/arm32-specific.h
	$(CC) $(JITFLAGS) myjit/jitlib-core.c
	
clean:
	rm *.o
	rm disasm/myjit-disasm
	rm ../bin/*