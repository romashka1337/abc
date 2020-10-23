#include <vector>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <cmath>

unsigned long dim;
unsigned short opt;
unsigned long bs;

long double dgemm(std::vector<std::vector<double>>& a, std::vector<std::vector<double>>& b,
		std::vector<std::vector<double>>& c) {
	auto start = std::chrono::system_clock::now();
	for (unsigned long it = 0; it < a.size(); ++it)
		for (unsigned long jt = 0; jt < a.size(); ++jt)
			for (unsigned long et = 0; et < a.size(); ++et)
				c[it][jt] += a[it][et] * b[et][jt];
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	return difference.count();
}

long double dgemm_opt_1(std::vector<std::vector<double>>& a, std::vector<std::vector<double>>& b,
		std::vector<std::vector<double>>& c) {
	auto start = std::chrono::system_clock::now();
	for (unsigned long it = 0; it < a.size(); ++it)
		for (unsigned long et = 0; et < a.size(); ++et)
			for (unsigned long jt = 0; jt < a.size(); ++jt)
				c[it][jt] += a[it][et] * b[et][jt];
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	return difference.count();
}

long double dgemm_opt_2(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c) {
	auto start = std::chrono::system_clock::now();
	for (unsigned long it = 0; it < dim; it += bs)
		for (unsigned long jt = 0; jt < dim; jt += bs)
			for (unsigned long et = 0; et < dim; et += bs)
				for (unsigned long it0 = 0; it0 < bs; ++it0)
					for (unsigned long et0 = 0; et0 < bs; ++et0)
						for (unsigned long jt0 = 0; jt0 < bs; ++jt0)
							c[(it0 + it) * dim + jt0 + jt] += a[(it0 + it) * dim + et0 + et] *
								 b[(et0 + et) * dim + jt0 + jt];
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	return difference.count();
}

long double dgemm_opt_3(std::vector<std::vector<double>>& __restrict__ a,
		std::vector<std::vector<double>>& __restrict__ b, std::vector<std::vector<double>>& __restrict__ c) {
	auto start = std::chrono::system_clock::now();
	// for (size_t i = 0; i < dim; ++i) {
	// 	for (size_t j = 0; j < dim; j += dim) {
	// 		std::vector<double> c_ij = a[i][0] * std::vector<double>(&b[0][0], dim);
	// 		for (size_t k = 1; k < dim; ++k) {
	// 			c_ij += a[i][k] * std::vector<double>(&b[k][0], dim);
	// 		}
	// 	}
	// }
	for (unsigned long it = 0; it < a.size(); ++it)
		for (unsigned long et = 0; et < a.size(); ++et)
	// #pragma GCC ivdep
			for (unsigned long jt = 0; jt < a.size(); ++jt)
				c[it][jt] += a[it][et] * b[et][jt];
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	return difference.count();
}

int main(int argc, char *argv[]) {
	// std::cout << sizeof(double) << std::endl;
	dim = std::stol(std::string(argv[1]));
	opt = std::stol(std::string(argv[2]));
	bs = std::stol(std::string(argv[3]));
	std::vector<std::vector<double>> a(dim, std::vector<double>(dim)), b(dim, std::vector<double>(dim)),
		c(dim, std::vector<double>(dim));
	std::vector<double> a0(dim * dim), b0(dim * dim), c0(dim * dim);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::normal_distribution<double> dist;
	for (auto& it : a)
		for (auto& _ : it)
			_ = dist(rng);
	for (auto& it : b)
		for (auto& _ : it)
			_ = dist(rng);
	for (auto& _ : a0)
		_ = dist(rng);
	for (auto& _ : b0)
		_ = dist(rng);
	long double time;
	if (opt == 0)
		time = dgemm(a, b, c);
	else if (opt == 1)
		time = dgemm_opt_1(a, b, c);
	else if (opt == 2)
		time = dgemm_opt_2(a0, b0, c0);
	else if (opt == 3)
		time = dgemm_opt_3(a, b, c);
	std::cout << time << std::endl;
}