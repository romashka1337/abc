#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <typeinfo>

std::ofstream out;
std::string pmodel;
std::string opt;

template<typename T>
void work(std::vector<T> tests, long long number_of_tests, long long power) {
	long double total_time = 0, variance = 0, sq_sum_of_times = 0;
	for (auto& _ : tests) {
		auto start = std::chrono::system_clock::now();
		std::pow(_, power);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<long double> difference = end - start;
		total_time += difference.count();
		sq_sum_of_times += difference.count() * difference.count();
	}
	long double average_time = total_time / number_of_tests;
	long double performance = std::log2(power) * number_of_tests / total_time;
	variance = (sq_sum_of_times - number_of_tests * average_time * average_time) / 
		(number_of_tests - 1);
	long double abs_error = sqrt(variance), rel_error = abs_error * 100 / average_time;
	out.precision(7);
	out << std::fixed << pmodel << ",pow" << power << "," << typeid(tests[0]).name()
		<< "," << opt << "," << number_of_tests << "," << std::log2(power) << "," << total_time
		<< "," << average_time << "," << abs_error << "," << rel_error << "," << performance
		<< std::endl;
	std::cout << std::setw(13) << "pow" + std::to_string(power) << std::setw(7) << typeid(tests[0]).name()
		<< std::setw(4) << opt << std::setw(9) << number_of_tests << " ";
	int diag = ceil(performance / 1e6);
	diag /= 5;
	diag--;
	for (auto it = 0; it < diag; ++it) std::cout << "|";
	std::cout << std::endl;
}

template<typename T>
void create_vectors(long long number_of_tests_1, long long number_of_tests_2, long long number_of_tests_3) {
	std::vector<T> tests_1(number_of_tests_1, 0), 
		tests_2(number_of_tests_2, 0), tests_3(number_of_tests_3, 0);
	std::random_device rd;
	std::mt19937 rng(rd());
	for (auto& _ : tests_1) _ = (T)rng();
	for (auto& _ : tests_2) _ = (T)rng();
	for (auto& _ : tests_3) _ = (T)rng();

	work<T>(tests_1, number_of_tests_1, 1e6);
	work<T>(tests_2, number_of_tests_2, 1e7);
	work<T>(tests_3, number_of_tests_3, 1e8);
}

int main(int argc, char *argv[]) {
	out.open(argv[1], std::ios_base::app);
	pmodel = argv[2];
	pmodel += " ";
	pmodel += argv[3];
	opt = argv[4];
	long long number_of_tests_1 = 0, number_of_tests_2 = 0, number_of_tests_3 = 0;
	std::cin >> number_of_tests_1 >> number_of_tests_2 >> number_of_tests_3;
	create_vectors<long double>(number_of_tests_1, number_of_tests_2, number_of_tests_3);
	create_vectors<double>(number_of_tests_1, number_of_tests_2, number_of_tests_3);
	create_vectors<long>(number_of_tests_1, number_of_tests_2, number_of_tests_3);
	return 0;
}
/*
Task	     OpType	Opt	LNum	0112233445566778899111111111111111111112222222
								5050505050505050505001122334455667788990011223
								___________________050505050505050505050505050
pow1000000   e		O0	999999	||
pow10000000	 e		O0	999999	|||
pow100000000 e		O0	999999	||||
*/