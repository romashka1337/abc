#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <typeinfo>
#include <type_traits>
#include <cmath>
#include <cstring>
#include <vector>

std::string memory_type;
std::string block_size = "1024/1b";
unsigned long launch_count;
char dim;
unsigned long size;
unsigned long bytes;
std::ofstream p1;

template<typename T>
std::pair<long double, long double> file(std::string path, std::vector<T> data, std::vector<T> data1) {
	long double total_write_time = 0, total_read_time = 0;
	std::ofstream out(path, std::ios_base::out | std::ios_base::binary);
	for (auto _ : data) {
		auto start = std::chrono::system_clock::now();
		out.write(reinterpret_cast<const char *>(&_), sizeof(int));
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<long double> difference = end - start;
		total_write_time += difference.count();
	}
	out.close();
	std::ifstream in(path, std::ios_base::in | std::ios_base::binary);
	for (auto _ : data1) {
		auto start = std::chrono::system_clock::now();
		in.read(reinterpret_cast<char *>(&_), sizeof(_));
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<long double> difference = end - start;
		total_read_time += difference.count();
	}
	in.close();
	system(std::string("rm " + path).c_str());
	return std::make_pair(total_read_time, total_write_time);
}

template<typename T>
void storeTest(std::vector<T> & data, std::vector<T> & bigdata) {
	bigdata.insert(bigdata.end(), data.begin(), data.end());
}

template<typename T>
void loadTest(std::vector<T> & data, std::vector<T> & bigdata) {
	data.assign(bigdata.begin(), bigdata.begin() + data.size());
}
template<typename T>
std::pair<long double, long double> testStoreLoad(std::vector<T> & data) {
	long double total_write_time = 0, total_read_time = 0;
	std::vector<T> bigdata;
	long long M = size * bytes / sizeof(data[0]), N = size * bytes * 2 / sizeof(data[0]);
	bigdata.reserve(N);

	auto start = std::chrono::system_clock::now();
	storeTest(data, bigdata);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<long double> difference = end - start;
	total_write_time += difference.count();

	start = std::chrono::system_clock::now();
	loadTest(data, bigdata);
	end = std::chrono::system_clock::now();
	difference = end - start;
	total_read_time += difference.count();

	return std::make_pair(total_read_time, total_write_time);
}

template<typename T>
void start() {
	if (dim == 'b') bytes = 1;
	else if (dim == 'K') bytes = 1024;
	else if (dim == 'M') bytes = 1024 * 1024;
	std::vector<T> data(bytes * size / sizeof(data[0])), data1(bytes * size / sizeof(data[0]));
	std::random_device rd;
	std::mt19937 rng(rd());
	for (unsigned long it = 0; it < bytes * size / sizeof(data[0]); ++it)
		data[it] = (T)rng();
	long double total_write_time = 0, total_read_time = 0, sq_sum_of_read_times = 0,
		sq_sum_of_write_times = 0;
	for (auto it = 0; it < launch_count; ++it) {
		std::pair<long double, long double> temp = std::make_pair(-1, -1);
		if (memory_type == "HDD" or memory_type == "SSD")
			temp = file<T>("temp", data, data1);
		else if (memory_type == "flash")
			temp = file<T>("/media/collapsing/AAAAAA/temp", data, data1);
		else if (memory_type == "RAM")
			temp = testStoreLoad<T>(data);
		total_read_time += temp.first;
		sq_sum_of_read_times += temp.first * temp.first;
		total_write_time += temp.second;
		sq_sum_of_write_times += temp.second * temp.second;
///////////////////////////////////////////////////////////////////////////////////////////
		long double average_write_time = total_write_time / (it + 1), 

			average_read_time = total_read_time / (it + 1),
		
			abs_write_error = sqrt((sq_sum_of_write_times - (it + 1) * average_write_time
			* average_write_time) / (it + 1)),
		
			rel_write_error = abs_write_error * 100 / average_write_time,
		
			abs_read_error = sqrt((sq_sum_of_read_times - (it + 1) * average_read_time
			* average_read_time) / (it + 1)),
		
			rel_read_error = abs_read_error * 100 / average_read_time,
		
			write_bandwidth = bytes * size / (average_write_time * 1024 * 1024),
		
			read_bandwidth = bytes * size / (average_read_time * 1024 * 1024);
		
		p1 << std::fixed << memory_type << "," << block_size << "," << typeid(data[0]).name()
			<< "," << sizeof(data[0]) << "," << it + 1 << ",std::chrono::system_clock::now(),"
			<< temp.second << "," << average_write_time << "," << write_bandwidth
			<< "," << abs_write_error << "," << rel_write_error << "," << temp.first
			<< "," << average_read_time << "," << read_bandwidth << "," << abs_read_error
			<< "," << rel_read_error << std::endl;
///////////////////////////////////////////////////////////////////////////////////////////
	}
	std::cout << total_read_time << " " << total_write_time << std::endl;
}

int main(int argc, char *argv[]) {
	if (std::string(argv[1]) == "-m" or std::string(argv[1]) == "--memory_type")
		memory_type = argv[2];
	else {
		std::cerr << "No memory type\n";
		return -1;
	}
	if (std::string(argv[3]) == "-b" or std::string(argv[3]) == "--block_size") {
		block_size = argv[4];
		std::size_t found = block_size.find("/");
		if (found == std::string::npos) {
			dim = 'b';
			size = std::stol(block_size);
		} else {
			dim = block_size[found + 2];
			size = std::stol(block_size.substr(0, found));
		}
		if (dim != 'b' and dim != 'K' and dim != 'M') {
			std::cerr << "Incorrect size\n";
			return -1;
		}
	} else {
		std::cerr << "No block size\n";
		return -1;
	}
	if (std::string(argv[5]) == "-l" or std::string(argv[5]) == "--launch_count")
		launch_count = std::stol(argv[6]);
	else {
		std::cerr << "No launch count\n";
		return -1;
	}
	p1.open(std::string(argv[7]), std::ios_base::app);
	if (std::string(argv[8]) == "char")
		start<char>();
	else if (std::string(argv[8]) == "int")
		start<int>();
	else if (std::string(argv[8]) == "long")
		start<long>();
	else if (std::string(argv[8]) == "ll")
		start<long long>();
	return 0;
}