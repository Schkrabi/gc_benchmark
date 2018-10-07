#!/bin/bash

tests=(BinTreeI BinTreeII BinTreeIII BinTreeIV GraphI GraphII GraphIII GraphIV)

for test in ${tests[@]};
do
    gnuplot -c runPlotAVG.gp "../archive/aggResults_${test}_cheney.csv" "../archive/aggResults_${test}_custom.csv" "../archive/aggResults_${test}_generated.csv" "../plots/${test}_AVG.pdf"
    gnuplot -c runPlotMED.gp "../archive/aggResults_${test}_cheney.csv" "../archive/aggResults_${test}_custom.csv" "../archive/aggResults_${test}_generated.csv" "../plots/${test}_MED.pdf"
done