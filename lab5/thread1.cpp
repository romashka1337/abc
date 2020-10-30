#include <omp.h>
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

void dgemm() {
	unsigned long it, jt, et;
	omp_set_num_threads(t_num);
#pragma omp parallel private(it, jt, et)
	{
		unsigned short core = step_it++;
		for (it = core * dim / t_num; it < (core + 1) * dim / t_num; ++it)
			for (jt = 0; jt < dim; ++jt)
				for (et = 0; et < dim; ++et)
					c[it * dim + jt] += a[it * dim + et] * b[et * dim + jt];
	}
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

	dgemm();
	
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	max_t_time = std::max(max_t_time, difference.count());

	std::ofstream out("dat/thread1", std::ios_base::app);
	out << std::fixed << max_t_time << "\t" << t_num << std::endl;
	return 0;
}