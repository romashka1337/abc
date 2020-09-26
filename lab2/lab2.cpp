#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <typeinfo>
#include <type_traits>

std::ofstream out;
std::string pmodel;
std::string opt;
std::ofstream average_performace_file;

template<typename T>
long double work(std::vector<T> tests, long long number_of_tests, long long power) {
	long double total_time = 0, variance = 0, sq_sum_of_times = 0;
	for (auto& _ : tests) { // for each test in tests vector
		auto start = std::chrono::system_clock::now(); // start timer
		std::pow(_, power); // calculate power function
		auto end = std::chrono::system_clock::now(); // stop timer
		std::chrono::duration<long double> difference = end - start; // calculate time of computation
		total_time += difference.count(); // total time for this task
		sq_sum_of_times += difference.count() * difference.count(); // to calculate errors later on
	}
	long double average_time = total_time / number_of_tests;
	long double performance = std::log2(power) * number_of_tests / total_time; // total operations per total time
	variance = (sq_sum_of_times - number_of_tests * average_time * average_time) / 
		(number_of_tests - 1); // veriance to calculate errors
	long double abs_error = sqrt(variance), rel_error = abs_error * 100 / average_time; // errors
	// outuput to .csv file; typeid(tests[0]).name() returns type name for task
	out << std::fixed << pmodel << ",pow" << power << "," << typeid(tests[0]).name()
		<< "," << opt << "," << number_of_tests << "," << std::setprecision(3) << std::log2(power)
		<< "," << std::setprecision(7) << total_time << "," << std::setprecision(7) << average_time
		<< "," << std::setprecision(7) << abs_error << "," << std::setprecision(3) << rel_error << ","
		<< std::setprecision(0) << performance << std::endl;
// diagram
	std::cout << std::setw(13) << "pow" + std::to_string(power) << std::setw(7) << typeid(tests[0]).name()
		<< std::setw(4) << opt << std::setw(9) << number_of_tests << " "; // specifications
	int diag = ceil(performance / 1e6); // use only millions
	diag /= 5; // scale if five mil
	diag--; // 0 mil doesnt count
	for (auto it = 0; it < diag; ++it) std::cout << "|"; // for each five mil print one '|'
	std::cout << std::endl; // next line
	return performance;
}

template<typename T> // template is used to work with different variables types
long double create_vectors(long long number_of_tests_1, long long number_of_tests_2, long long number_of_tests_3) {
	std::vector<T> tests_1(number_of_tests_1, 0), // create vectors of data for each task
		tests_2(number_of_tests_2, 0), tests_3(number_of_tests_3, 0);
	std::random_device rd;                       //
	std::mt19937 rng(rd());                      //
	if (std::is_same<T, int>::value) {           // normal distribution doesnt work for ints
		for (auto& _ : tests_1) _ = (T)rng();    // fill vectors with random
		for (auto& _ : tests_2) _ = (T)rng();    // data of given type
		for (auto& _ : tests_3) _ = (T)rng();    //
	} else if (std::is_same<T, float>::value) {  //
		std::normal_distribution<float> dist;    // 
		for (auto& _ : tests_1) _ = dist(rng);   // fill vectors with random
		for (auto& _ : tests_2) _ = dist(rng);   // data of given type
		for (auto& _ : tests_3) _ = dist(rng);   //
	} else if (std::is_same<T, double>::value) { //
		std::normal_distribution<double> dist;   //
		for (auto& _ : tests_1) _ = dist(rng);   // fill vectors with random
		for (auto& _ : tests_2) _ = dist(rng);   // data of given type
		for (auto& _ : tests_3) _ = dist(rng);   //
	}
	long double average_performace = 0;
	average_performace += work<T>(tests_1, number_of_tests_1, 1e6); //
	average_performace += work<T>(tests_2, number_of_tests_2, 1e7); // calculate average performance
	average_performace += work<T>(tests_3, number_of_tests_3, 1e8); // to write into .csv file
	average_performace /= 3;                                        //
	return average_performace;
}

int main(int argc, char *argv[]) {
	out.open(argv[1], std::ios_base::app); // open .csv file
	pmodel = argv[2];  //
	pmodel += " ";     // pmodel -> processor model
	pmodel += argv[3]; //
	opt = argv[4]; // optimization flag
	average_performace_file.open(argv[5], std::ios_base::app); // open .csv file for average performances
	long long number_of_tests_1 = 0, number_of_tests_2 = 0, number_of_tests_3 = 0;
	std::cin >> number_of_tests_1 >> number_of_tests_2 >> number_of_tests_3; // input number of test
	long double average_performace = 0;
	average_performace += create_vectors<int>(number_of_tests_1, number_of_tests_2, number_of_tests_3);
	average_performace += create_vectors<float>(number_of_tests_1, number_of_tests_2, number_of_tests_3);
	average_performace += create_vectors<double>(number_of_tests_1, number_of_tests_2, number_of_tests_3);
	average_performace /= 3;
	average_performace_file.precision(0); // output average performance to .csv file
	average_performace_file /*<< typeid(tests_1[0]).name() << "," */<< opt << "," << std::fixed
		<< average_performace << std::endl;
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