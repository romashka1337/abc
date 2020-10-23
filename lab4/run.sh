#!/bin/bash

# l1d 8192
# l1i 16384
# l2 131072
bs=128

# mkdir data
# mkdir p3

# sudo apt-get install -y gnuplot

g++ -std=c++2a dgemm.cpp -o dgemm

for it in {512..2048..512}
do
	for jt in {0..3..1}
	do
		./dgemm $it $jt $bs
	done
done

# g++ -std=c++17 plots.cpp -o plots
# ./plots
# gnuplot plot.plg