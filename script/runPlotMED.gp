#!/usr/bin/gnuplot

set term pdf
set output ARG4
set ylabel "rdtsc"
set xlabel ARG5

plot ARG1 using 6:3 lt rgb "red" title "Cheney MED", ARG2 using 6:3 lt rgb "blue" title "Custom MED", ARG3 using 6:3 lt rgb "green" title "Generated MED"
