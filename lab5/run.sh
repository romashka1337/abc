#!/bin/bash

array=(2 4 8 16 32 64 128)
dim=(512 1024)

g++ -std=c++2a -pthread thread0.cpp -o thread0
g++ -std=c++2a -fopenmp thread1.cpp -o thread1
g++ -std=c++2a thread2.cpp -o thread2 -ltbb
mpic++ -std=c++2a thread3.cpp -o thread3

./thread0 ${dim[0]} 1
./thread0 ${dim[1]} 1

for et in ${dim[*]}
do
	for it in ${array[*]}
	do
		./thread0 $et $it >> trash/thread0_$et
	done
done

for et in ${dim[*]}
do
	for it in ${array[*]}
	do
		./thread1 $et $it >> trash/thread1_$et
		./thread2 $et $it >> trash/thread2_$et

		temp=$(($it + 1))
		mpirun --oversubscribe -np $temp ./thread3 $et $temp >> trash/thread3_$et
	done
done

g++ -std=c++2a plots.cpp -o plots
./plots
gnuplot plot