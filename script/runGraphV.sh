#!/bin/bash

round()
{
    echo $(printf %.$2f $(echo "scale=$2;(((10^$2)*$1)+0.5)/(10^$2)" | bc))
};

gcs=(cheney custom generated)
testName=GraphI
test=graph_multitype
logDir=../log/
dataDir=../data/
archiveDir=../archive/

#Clean the logs and data
rm -rf ${logDir}*
rm -rf ${dataDir}*

e=13
O=15

M=$((2**e))
#type table test
for G in {1..400..10}
do
    Y=5
    X=$(((M/(32 + (Y * 32) + ((Y-1)*40)))*100))
    C=0.01
    
    testId=${testName}_G={$G}
    echo $testId

    it=0
    while [ "$it" -ne ${1-"1"} ];
    do
        seed=$RANDOM
        echo $seed
        for gc in ${gcs[@]};
        do
            #echo ../bin/gc_benchmark -s $seed -t $test -g $gc -m $M -q $X -a $Y -c $C -o $O -x $G
            ../bin/gc_benchmark -s $seed -t $test -g $gc -m $M -q $X -a $Y -c $C -o $O -x $G
        done
        it=$((it+1))
    done    
    
    #Prepare the data and run the statistics
    for gc in ${gcs[@]}; 
    do
        dataFile=${dataDir}${gc}_${testId}.csv
        echo UNIT_START,ID,CLOCK_START,BYTES_START,UNIT_END,ID2,CLOCK_END,BYTES_END,VAR > $dataFile
        find ${logDir}log_${test}_${gc}* | xargs cat | grep -E *C[SE]* | sed -e 's/ C[SE] /,/' -e 's/ /,/g' | sed -e 'N;s/\n/,/' | sed -e "s/.*/&,$Z/" >> $dataFile
        rsltFile=../results/result_${gc}_${testName}_Z=${Z}
        Rscript doStatistics.R $dataFile $rsltFile
    done
    
    #Evacuate logs & data
    dateTime=$(date +%Y-%m-%d:%H:%M:%S)
    evacuateDir=${archiveDir}${testName}_${M}B_Z=${Z}_${dateTime}
    if [ ! -d ${evacuateDir} ]; then
        mkdir -p ${evacuateDir};
    fi

    mv ${logDir}* ${evacuateDir}
    mv ${dataDir}* ${evacuateDir}
    mv ../results/* ${evacuateDir}
done
