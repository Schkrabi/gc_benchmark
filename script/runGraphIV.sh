#!/bin/bash

round()
{
    echo $(printf %.$2f $(echo "scale=$2;(((10^$2)*$1)+0.5)/(10^$2)" | bc))
};

gcs=(cheney custom generated)
testName=GraphIV
test=graph
logDir=../log/
dataDir=../data/
archiveDir=../archive/

#Clean the logs and data
rm -rf ${logDir}*
rm -rf ${dataDir}*

#Memory test
for e in {13..23} #Iterate memory over 2**13 to 2**23 bytes
#for e in {13..14} #TODO REMOVE  
do
    M=$((2**e))
    
    Y=10
    X=$(((M/(Y*32))*100))
    O=$(echo "((($M/(32 + ($Y * 32) + (($Y-1)*40)))/100)*50)" | bc -l)
    O=$(round $O 0)
    O=$(($O>1 ? $O : 1))
    
    C=0.01
    
    testId=${testName}_M=${M}
    echo $testId
    
    it=0
    while [ "$it" -ne ${1-"1"} ];
    do
        seed=$RANDOM
        echo $seed
        for gc in ${gcs[@]};
        do
            #echo ../bin/gc_benchmark -s $seed -t $test -g $gc -m $M -q $X -a $Y -c $C -o $O
            ../bin/gc_benchmark -s $seed -t $test -g $gc -m $M -q $X -a $Y -c $C -o $O
        done
        it=$((it+1))
    done    
    
    #Prepare the data and run the statistics
    for gc in ${gcs[@]}; 
    do
        dataFile=${dataDir}${gc}_${testId}.csv
        echo UNIT_START,NSEC_START,ID,CLOCK_START,BYTES_START,UNIT_END,NSEC_END,ID2,CLOCK_END,BYTES_END,VAR > $dataFile
        find ${logDir}log_${test}_${gc}* | xargs cat | grep -E *C[SE]* | sed -e 's/ C[SE] /,/' -e 's/ /,/g' | sed -e 'N;s/\n/,/' | sed -e "s/.*/&,$M/" >> $dataFile
        rsltFile=../results/result_${gc}_${testName}_M=${M}
        Rscript doStatistics.R $dataFile $rsltFile
    done
    
    #Evacuate logs & data
    dateTime=$(date +%Y-%m-%d:%H:%M:%S)
    evacuateDir=${archiveDir}${testName}_${M}B_${dateTime}
    if [ ! -d ${evacuateDir} ]; then
        mkdir -p ${evacuateDir};
    fi

    mv ${logDir}* ${evacuateDir}
    mv ${dataDir}* ${evacuateDir}
    mv ../results/* ${evacuateDir}
done
