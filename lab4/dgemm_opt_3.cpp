#include <vector>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>

unsigned long dim;
unsigned short opt;
unsigned long bs;

long double dgemm_opt_3(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c) {
	auto start = std::chrono::system_clock::now();
	for (unsigned long it = 0; it < dim; ++it)
		for (unsigned long et = 0; et < dim; ++et)
			for (unsigned long jt = 0; jt < dim; ++jt)
				c[it * dim + jt] += a[it * dim + et] * b[et * dim + jt];
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	std::ofstream out("opt_3", std::ios_base::app);
	out << std::fixed << difference.count() << "\t" << dim << std::endl;
	return difference.count();
}

int main(int argc, char *argv[]) {
	dim = std::stol(std::string(argv[1]));
	std::vector<double> a0(dim * dim), b0(dim * dim), c0(dim * dim);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::normal_distribution<double> dist;
	for (auto& _ : a0)
		_ = dist(rng);
	for (auto& _ : b0)
		_ = dist(rng);
	long double time;
	time = dgemm_opt_3(a0, b0, c0);
	std::cout << time << std::endl;
}