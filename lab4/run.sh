!/bin/bash

bs=128

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
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "opt_0\t%s\n", $1}' >> misses
sudo perf stat -e cache-misses ./dgemm 512 1 128 2> a
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "opt_1\t%s\n", $1}' >> misses
sudo perf stat -e cache-misses ./dgemm 512 2 128 2> a
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "opt_2\t%s\n", $1}' >> misses
sudo perf stat -e cache-misses ./dgemm_opt_3 512 2> a
cat a | echo $(egrep -m 1 'cache-misses') | awk '{printf "opt_3\t%s\n", $1}' >> misses

sed '5,$d' opt_0 > opt_0_sp
cat opt_0_sp > opt_0
sed '5,$d' opt_1 > opt_1_sp
cat opt_1_sp > opt_1
sed '5,$d' opt_2 > opt_2_sp
cat opt_2_sp > opt_2
sed '5,$d' opt_3 > opt_3_sp
cat opt_3_sp > opt_3

rm opt_0_sp opt_1_sp opt_2_sp opt_3_sp

echo -e "$(./dgemm 512 2 64)\t64" >> bs
echo -e "$(./dgemm 512 2 128)\t128" >> bs
echo -e "$(./dgemm 512 2 256)\t256" >> bs

g++ -std=c++2a plots.cpp -o plots
./plots
gnuplot plot

rm opt_1 opt_2 opt_3