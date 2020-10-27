#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

std::ifstream in;
std::ofstream out;
std::vector<double> opt_0(4);

int main() {
	std::string str1, str2;
	in.open("opt_0");
	for (auto it = 0; it < 4; ++it) {
		in >> str1 >> str2;
		opt_0[it] = std::stod(str1);
	}
	in.close();
	in.open("opt_1");
	out.open("opt_1_sp", std::ios_base::app);
	for (auto it = 0; it < 4; ++it) {
		in >> str1 >> str2;
		out << opt_0[it] / std::stod(str1) << "\t" << str2 << std::endl;
	}
	out.close();
	in.close();
	in.open("opt_2");
	out.open("opt_2_sp", std::ios_base::app);
	for (auto it = 0; it < 4; ++it) {
		in >> str1 >> str2;
		out << opt_0[it] / std::stod(str1) << "\t" << str2 << std::endl;
	}
	out.close();
	in.close();
	in.open("opt_3");
	out.open("opt_3_sp", std::ios_base::app);
	for (auto it = 0; it < 4; ++it) {
		in >> str1 >> str2;
		out << opt_0[it] / std::stod(str1) << "\t" << str2 << std::endl;
	}
	out.close();
	in.close();
	return 0;
}