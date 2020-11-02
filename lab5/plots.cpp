#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

std::ifstream in;
std::ofstream out;

int main() {
	std::string str1, str2;
	in.open("trash/comp");
	in >> str1 >> str2;
	long double _512 = std::stod(str1), _1024 = std::stod(str2), temp;
	in.close();
	for (auto it = 512; it <= 1024; it += 512)
		for (auto jt = 0; jt < 4; ++jt) {
			str1 = "thread" + std::to_string(jt) + "_" + std::to_string(it);
			in.open("trash/" + str1);
			out.open("dat/" + str1, std::ios_base::app);
			it % 1024 ? temp = _512 : temp = _1024;
			for (auto et = 0; et < 7; ++et) {
				in >> str1 >> str2;
				out << temp / std::stod(str1) << "\t" << str2 << std::endl;
			}
			out.close();
			in.close();
		}
	return 0;
}