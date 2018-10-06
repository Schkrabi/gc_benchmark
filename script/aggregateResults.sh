#!/bin/bash

gcs=(cheney custom generated)
tests=(BinTreeI BinTreeII BinTreeIII BinTreeIV GraphI GraphII GraphIII GraphIV)

for gc in ${gcs[@]};
do
    #file=../archive/aggResults_${gc}_$(date +%Y-%m-%d:%H:%M:%S).csv
    for test in ${tests[@]};
    do
        file=../archive/aggResults_${test}_${gc}.csv
        echo GARBAGE_COLLECTOR\;MEAN_rdtsc\;MEDIAN_rdtsc\;MEAN_clock\;MEDIAN_clock\;VAR | tr ';' '\t' > $file
        for dir in ../archive/${test}_*/
        do
            #echo ${dir} | sed -e "s/\.\.\/archive\///" >> $file
            for f in ${dir}result_${gc}*
            do
                #gc=$(echo $f | grep -Eo 'cheney|generated|custom')
                cat $f | tr ' ' '\t' | sed -e "s/^/$gc\t/" >> $file
            done
        done
    done
done
