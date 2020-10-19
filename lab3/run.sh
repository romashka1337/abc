#!/bin/bash

mkdir data
mkdir p3

sudo apt-get install -y gnuplot

g++ memory_test.cpp -o memory_test

for i in {1..101..50}
do
	./memory_test -m RAM -b $i/1Mb -l 10 p1.csv char
	./memory_test -m HDD -b $i/1Mb -l 10 p1.csv char
	./memory_test -m flash -b $i/1Mb -l 10 p1.csv char
done

./memory_test -m RAM -b 100/1Mb -l 10 p2.csv char
./memory_test -m RAM -b 1/1Mb -l 10 p2.csv char
./memory_test -m RAM -b 160/1Kb -l 10 p2.csv char
./memory_test -m RAM -b 64/1b -l 10 p2.csv char

for i in {4..80..4}
do
	./memory_test -m HDD -b $i/1Mb -l 10 p2.csv char
done

./memory_test -m HDD -b 4/1Mb -l 10 p2.csv char
./memory_test -m flash -b 4/1Mb -l 10 p2.csv char

./memory_test -m HDD -b 4/1Mb -l 10 p2.csv int
./memory_test -m flash -b 4/1Mb -l 10 p2.csv int

./memory_test -m HDD -b 4/1Mb -l 10 p2.csv long
./memory_test -m flash -b 4/1Mb -l 10 p2.csv long

./memory_test -m HDD -b 4/1Mb -l 10 p2.csv ll
./memory_test -m flash -b 4/1Mb -l 10 p2.csv ll

g++ -std=c++17 plots.cpp -o plots
./plots
gnuplot plot.plg