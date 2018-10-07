#!/usr/bin/gnuplot

set term pdf
set output ARG4

plot ARG1 using 2:6 lt rgb "red" with lp title "Cheney AVG", ARG2 using 2:6 lt rgb "blue" with lp title "Custom AVG", ARG3 using 2:6 lt rgb "green" with lp title "Generated AVG"