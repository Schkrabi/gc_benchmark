#!/bin/bash


file=../archive/masterResults$(date +%Y-%m-%d:%H:%M:%S).csv

echo GARBAGE_COLLECTOR\;MEAN_rdtsc\;MEDIAN_rdtsc\;MEAN_clock\;MEDIAN_clock > $file

for dir in ../archive/*/
do
    echo ${dir} | sed -e "s/\.\.\/archive\///" >> $file
    for f in ${dir}result_*
    do
        gc=$(echo $f | sed -e "s/^\.\.\/archive\/[A-Za-z0-9\_:-=]*\/result\_//" | sed -e 's/_.*$//')
        cat $f | tr '.' ',' | tr ' ' ';' | sed -e "s/^/$gc;/" >> $file
    done
done
