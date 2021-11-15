#!/bin/bash

for N in {1..50}
do
    echo /home/zdx/repo/mts/test/$((($N % 6)+1)).c
    ./client /home/zdx/repo/mts/test/$((($N % 6)+1)).c
done
wait
