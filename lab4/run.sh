#!/bin/bash

bs=128

sudo apt-get install -y gnuplot

g++ -std=c++2a dgemm.cpp -o dgemm
g++ -std=c++2a -O3 dgemm_opt_3.cpp -o dgemm_opt_3

for it in {512..2048..512}
do
	for jt in {0..2..1}
	do
		./dgemm $it $jt $bs
	done
	./dgemm_opt_3 $it
done

sudo perf stat -e cache-misses ./dgemm 512 0 128 2> a
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "%s\t0\n", $1}' >> misses
sudo perf stat -e cache-misses ./dgemm 512 1 128 2> a
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "%s\t1\n", $1}' >> misses
sudo perf stat -e cache-misses ./dgemm 512 2 128 2> a
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "%s\t2\n", $1}' >> misses
sudo perf stat -e cache-misses ./dgemm_opt_3 512 2> a
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "%s\t3\n", $1}' >> misses

gnuplot plot