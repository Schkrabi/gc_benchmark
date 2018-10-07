#!/usr/bin/gnuplot

set term pdf
set output ARG4

plot ARG1 using 3:6 lt rgb "red" with lp title "Cheney MED", ARG2 using 3:6 lt rgb "blue" with lp title "Custom MED", ARG3 using 3:6 lt rgb "green" with lp title "Generated MED"
