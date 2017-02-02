#!/bin/bash

gcs=(cheney custom)
tests=(short_lived long_lived)

#Run the experiment
it=0
while [ "$it" -ne ${1-"1"} ]; do
    seed=$RANDOM
    for test in ${tests[@]}; do
        for gc in ${gcs[@]}; do
            ./gc_benchmark -s $seed -t $test -g $gc 
        done
    done    
    it=$((it+1))
done

#Prepare the data and run the statistics
logDir=../log/
dataDir=../data/

for test in ${tests[@]}; do
    for gc in ${gcs[@]}; do
        dataFile=${dataDir}${gc}_${test}.csv
        echo SEC_START,NSEC_START,ID,CLOCK_START,BYTES_START,SEC_END, NSEC_END, ID2, CLOCK_END, BYTES_END > $dataFile
        find ${logDir}log_${test}_${gc}* | xargs cat | grep -E *C[SE]* | sed -e 's/ C[SE] /,/' -e 's/ /,/g' | sed -e 'N;s/\n/,/'  >> $dataFile
        Rscript doStatistics.R $dataFile ../results/result_${gc}_${test}
    done
done