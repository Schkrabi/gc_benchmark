#!/bin/bash

tests=(BinTreeI BinTreeII BinTreeIII BinTreeIV GraphI GraphII GraphIII GraphIV)

for test in ${tests[@]};
do
    if ([ "$test" == "BinTreeI" ] || [ "$test" == "GraphI" ])
    then
        ylabel="living(%)"
    elif ([ "$test" == "BinTreeII" ] || [ "$test" == "GraphII" ])
    then
        ylabel="objectSize(nodes)"
    elif ([ "$test" == "BinTreeIII" ] || [ "$test" == "GraphIII" ])
    then
        ylabel="chanceToReplace(%)"
    else
        ylabel="memory(B)"
    fi
    
    gnuplot -c runPlotAVG.gp "../archive/aggResults_${test}_cheney.csv" "../archive/aggResults_${test}_custom.csv" "../archive/aggResults_${test}_generated.csv" "../plots/${test}_AVG.pdf" $ylabel
    gnuplot -c runPlotMED.gp "../archive/aggResults_${test}_cheney.csv" "../archive/aggResults_${test}_custom.csv" "../archive/aggResults_${test}_generated.csv" "../plots/${test}_MED.pdf" $ylabel
done
