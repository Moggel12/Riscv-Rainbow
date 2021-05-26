#! /bin/bash

if [ $# -lt 2 ]
then
    echo "Please provide the following as arguments:"
    echo "1) The path to the pts device running the simulator"
    echo "2) Whether or not this is a benchmarking run or a test run ('bench', 'correct' or amount of new tests)"
else
    if [[ $2 =~ ^[1-9][0-9]*$ ]]
    then
    echo "Running $2 KATs"
        for (( i=0; i<$2; i++ ))
        do
            ./kat.py -u $1
        done
    elif [ $2 = "correct" ]
    then
        echo "Testing existing KATs.."
        for f in KAT*
        do
            i=$(cut -d_ -f2 <<< $f)
            ./kat.py -u $1 -c $i
        done
    elif [ $2 = "bench" ]
    then
        select impl in standard bitslice lookup
        do
            echo "Benchmarking existing KATs.."
            for f in KAT*
            do
                i=$(cut -d_ -f2 <<< $f)
                ./kat.py -u $1 -c $i -b
                mkdir -p KAT_$i/$impl
                mv KAT_$i/benchmarks KAT_$i/$impl/benchmarks
            done
            break
        done
    else
        echo "You did not specify 'bench', 'correct' or an integer as the second argument"
    fi
fi