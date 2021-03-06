#!/bin/bash

round()
{
    echo $(printf %.$2f $(echo "scale=$2;(((10^$2)*$1)+0.5)/(10^$2)" | bc))
};

function experiment
{
    O=15
    e=13
    M=$((2**e))
    B=$2
    
    Y=5
    X=$(((M/(${graph_size} + (Y * ${graph_node_size}) + ((Y-1)*${graph_edge_size})))*100))
    C=0.01
    
    testId=${testName}_B=${B}
    echo $testId

    it=0
    while [ "$it" -ne ${1-"1"} ];
    do
        seed=$RANDOM
        echo $seed
        for gc in ${gcs[@]};
        do
            #echo ../bin/gc_benchmark -s $seed -t $test -g $gc -m $M -q $X -a $Y -c $C -o $O
            ../bin/gc_benchmark_${B}_graphs  -s $seed -t $test -g $gc -m $M -q $X -a $Y -c $C -o $O
        done
        it=$((it+1))
    done    
    
    #Prepare the data and run the statistics
    for gc in ${gcs[@]}; 
    do
        dataFile=${dataDir}${gc}_${testId}.csv
        echo UNIT_START,NSEC_START,ID,CLOCK_START,BYTES_START,UNIT_END,NSEC_END,ID2,CLOCK_END,BYTES_END,VAR > $dataFile
        find ${logDir}log_${test}_${gc}* | xargs cat | grep -E *C[SE]* | sed -e 's/ C[SE] /,/' -e 's/ /,/g' | sed -e 'N;s/\n/,/' | sed -e "s/.*/&,$B/" >> $dataFile
        rsltFile=../results/result_${gc}_${testName}_B=${B}
        Rscript doStatistics.R $dataFile $rsltFile
    done
    
    #Evacuate logs & data
    dateTime=$(date +%Y-%m-%d:%H:%M:%S)
    evacuateDir=${archiveDir}${testName}_${M}B_B=${B}_${dateTime}
    if [ ! -d ${evacuateDir} ]; then
        mkdir -p ${evacuateDir};
    fi

    mv ${logDir}* ${evacuateDir}
    mv ${dataDir}* ${evacuateDir}
    mv ../results/* ${evacuateDir}
}

gcs=(cheney custom generated)
testName=GraphV
test=graph_multitype
logDir=../log/
dataDir=../data/
archiveDir=../archive/

#Clean the logs and data
rm -rf ${logDir}*
rm -rf ${dataDir}*

#const
graph_size=64
graph_node_size=64
graph_edge_size=72

#Run the experiment
#type table test
for B in {150..300..30} 
do
    experiment $1 $B   
done
for B in {450..1200..150} 
do
    experiment $1 $B
done

