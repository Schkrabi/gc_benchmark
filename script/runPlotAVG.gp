#!/usr/bin/gnuplot

set term pdf
set output ARG4
set ylabel "rdtsc"
set xlabel ARG5

plot ARG1 using 6:2 lt rgb "red" title "Cheney AVG", ARG2 using 6:2 lt rgb "blue" title "Custom AVG", ARG3 using 6:2 lt rgb "green" title "Generated AVG"
