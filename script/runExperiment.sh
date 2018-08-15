#!/bin/bash

gcs=(cheney custom generated)
tests=(short_lived long_lived long_lived_almost_full long_lived_no_replace large_structure)

#Run the experiment
it=0
while [ "$it" -ne ${1-"1"} ]; do
    seed=$RANDOM
    echo $seed
    for test in ${tests[@]}; do
        for gc in ${gcs[@]}; do
            ../bin/gc_benchmark -s $seed -t $test -g $gc 
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
        echo UNIT_START,ID,CLOCK_START,BYTES_START,UNIT_END,ID2,CLOCK_END,BYTES_END > $dataFile
        find ${logDir}log_${test}_${gc}* | xargs cat | grep -E *C[SE]* | sed -e 's/ C[SE] /,/' -e 's/ /,/g' | sed -e 'N;s/\n/,/'  >> $dataFile
        Rscript doStatistics.R $dataFile ../results/result_${gc}_${test}
    done
done