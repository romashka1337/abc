#!/bin/bash
cpu_model=$(lscpu | echo $(egrep -m 1 'Model name') | awk '{ print $3, $4}')

echo 'PModel,Task,OpType,Opt,LNum,InsCount,Timer,AvTime,AbsErr,RelErr,TaskPerf' > lab2.csv
echo '         Task OpType Opt    LNum  0112233445566778899111111111111111111112222222'
echo '                                  5050505050505050505001122334455667788990011223'
echo '                                  ___________________050505050505050505050505050'
g++ -std=c++17 -O0 lab2.cpp -o O0 && ./O0 < input lab2.csv $cpu_model O0
g++ -std=c++17 -O1 lab2.cpp -o O1 && ./O1 < input lab2.csv $cpu_model O1
g++ -std=c++17 -O2 lab2.cpp -o O2 && ./O2 < input lab2.csv $cpu_model O2
g++ -std=c++17 -O3 lab2.cpp -o O3 && ./O3 < input lab2.csv $cpu_model O3