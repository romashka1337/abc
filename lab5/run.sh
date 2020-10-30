#!/bin/bash

array=(1 2 4 8 16 32 64 128)
dim=512

g++ -std=c++2a -pthread thread0.cpp -o thread0
g++ -std=c++2a -fopenmp thread1.cpp -o thread1
g++ -std=c++2a -ltbb thread2.cpp -o thread2
mpic++ -std=c++2a thread3.cpp -o thread3

for it in ${array[*]}
do
	./thread0 $dim $it
done

for it in {1..2..1}
do
	./thread$it $dim ${array[1]}
done

mpirun --oversubscribe -np ${array[1]} ./thread3 $dim ${array[1]}