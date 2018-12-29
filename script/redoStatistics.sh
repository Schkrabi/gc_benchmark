#!/bin/bash

gcs=(cheney custom generated)

for dir in ../archive/*
do
    for gc in ${gcs[@]};
    do
        echo ${dir}/${gc}*
        f=$(find ${dir}/${gc}*)
        echo $f
        rsltFile=$(echo $f | sed -e "s/${gc}/result\_${gc}/" -e "s/\.csv//")
        echo $rsltFile
        rm $rsltFile
        Rscript doStatistics.R $f $rsltFile
    done
done
