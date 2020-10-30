#include <pthread.h>
#include <vector>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>

unsigned long dim;
unsigned short t_num;
unsigned short step_it = 0;
std::vector<double> a, b, c;
long double max_t_time = 0;

void* dgemm(void* arg) {
	unsigned short core = step_it++;
	for (unsigned long it = core * dim / t_num; it < (core + 1) * dim / t_num; ++it)
		for (unsigned long jt = 0; jt < dim; ++jt)
			for (unsigned long et = 0; et < dim; ++et)
				c[it * dim + jt] += a[it * dim + et] * b[et * dim + jt];
}

int main(int argc, char** argv) {
	dim = std::stol(std::string(argv[1]));
	t_num = std::stol(std::string(argv[2]));

	a.resize(dim * dim); b.resize(dim * dim); c.resize(dim * dim);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::normal_distribution<double> dist;
	for (auto& _ : a)
		_ = dist(rng);
	for (auto& _ : b)
		_ = dist(rng);

	auto start = std::chrono::system_clock::now();
	
	pthread_t threads[t_num];
	for (int it = 0; it < t_num; ++it)
		pthread_create(&threads[it], NULL, dgemm, NULL); 
	for (int it = 0; it < t_num; ++it)
		pthread_join(threads[it], NULL);
	
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	max_t_time = std::max(max_t_time, difference.count());

	std::ofstream out("dat/thread0", std::ios_base::app);
	out << std::fixed << max_t_time << "\t" << t_num << std::endl;
	return 0;
}