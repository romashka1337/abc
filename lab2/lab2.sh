#!/bin/bash
cpu_model=$(lscpu | echo $(egrep -m 1 'Model name') | awk '{ print $3, $4}')
csv_file=$(echo lab2.csv)
input_file=$(echo input)
average_performance_file=$(echo average_performance.csv)

clear
echo 'PModel,Task,OpType,Opt,LNum,InsCount,Timer,AvTime,AbsErr,RelErr,TaskPerf' > $csv_file
echo '         Task OpType Opt    LNum  0112233445566778899111111111111111111112222222'
echo '                                  5050505050505050505001122334455667788990011223'
echo '                                  ___________________050505050505050505050505050'
echo 'Opt,AvPerf' > $average_performance_file
g++ -std=c++17 -O0 lab2.cpp -o O0 && ./O0 < $input_file $csv_file $cpu_model O0 $average_performance_file
g++ -std=c++17 -O1 lab2.cpp -o O1 && ./O1 < $input_file $csv_file $cpu_model O1 $average_performance_file
g++ -std=c++17 -O2 lab2.cpp -o O2 && ./O2 < $input_file $csv_file $cpu_model O2 $average_performance_file
g++ -std=c++17 -O3 lab2.cpp -o O3 && ./O3 < $input_file $csv_file $cpu_model O3 $average_performance_file

rm O0 O1 O2 O3