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
#include <sstream>

std::ifstream in;
std::ofstream out;
std::ofstream out_csv;

class CSVRow {
	public:
		std::string_view operator[](std::size_t index) const {
			return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
		}
		std::size_t size() const {
			return m_data.size() - 1;
		}
		void readNextRow(std::istream& str) {
			std::getline(str, m_line);
			m_data.clear();
			m_data.emplace_back(-1);
			std::string::size_type pos = 0;
			while((pos = m_line.find(',', pos)) != std::string::npos) {
				m_data.emplace_back(pos);
				++pos;
			}
			// This checks for a trailing comma with no data after it.
			pos   = m_line.size();
			m_data.emplace_back(pos);
		}
	private:
		std::string m_line;
		std::vector<int> m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data) {
	data.readNextRow(str);
	return str;
}   

class CSVIterator {   
	public:
		typedef std::input_iterator_tag iterator_category;
		typedef CSVRow value_type;
		typedef std::size_t difference_type;
		typedef CSVRow* pointer;
		typedef CSVRow& reference;

		CSVIterator(std::istream& str) : m_str(str.good()?&str:NULL) {++(*this);}
		CSVIterator() :m_str(NULL) {}

		// Pre Increment
		CSVIterator& operator++() {
			if (m_str)
				if (!((*m_str) >> m_row)) m_str = NULL;
			return *this;
		}
		// Post increment
		CSVIterator operator++(int) {CSVIterator tmp(*this); ++(*this); return tmp;}
		CSVRow const& operator*() const {return m_row;}
		CSVRow const* operator->() const {return &m_row;}

		bool operator==(CSVIterator const& rhs) {
			return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));
		}
		bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}
	private:
		std::istream* m_str;
		CSVRow m_row;
};

class CSVRange {
	std::istream& stream;
	public:
		CSVRange(std::istream& str) : stream(str) {}
		CSVIterator begin() const {return CSVIterator{stream};}
		CSVIterator end() const {return CSVIterator{};}
};

void kek(std::string s1, std::string s2, int count, int num) {
	in.open(s1, std::ios_base::in);
	out.open(s2, std::ios_base::out);
	for (auto& row : CSVRange(in)) {
		if (count == 30) {
			std::size_t found = row[1].find("/");
			out << row[num] << "	" << row[1].substr(0, found) << std::endl;
			count = 1;
		} else ++count;
	}
	out.close();
	in.close();
}

void eke(std::string s1, std::string s2, int num) {
	in.open(s1, std::ios_base::in);
	out.open(s2, std::ios_base::out);
	for (auto& row : CSVRange(in))
		out << row[num] << "	" << row[0] << std::endl;
	out.close();
	in.close();
}

void kek1(std::string s1, std::string s2, int count, int num) {
	in.open(s1, std::ios_base::in);
	out.open(s2, std::ios_base::out);
	for (auto& row : CSVRange(in)) {
		if (count == 1) {
			out << row[num] << "	" << row[1] << std::endl;
			count = 0;
		} else ++count;
	}
	out.close();
	in.close();
}

int main(int argc, char *argv[]) {
	in.open("p1.csv", std::ios_base::in);
	out_csv.open("p3/p1_table.csv", std::ios_base::out);
	out_csv << "MemoryType,BlockSize,WriteBandwidth,ReadBandwidth\n";
	int count = 0;
	for (auto& row : CSVRange(in)) {
		if (count == 10) {
			out_csv << row[0] << "," << row[1] << "," << row[8] << "," << row[13] << std::endl;
			count = 1;
		} else ++count;
	}
	out_csv.close();
	in.close();
	in.open("p1.csv", std::ios_base::in);
	out_csv.open("p3/p2_table.csv", std::ios_base::out);
	out_csv << "MemoryType,BlockSize,AbsError(write),RelError(write),AbsError(read),RelError(read)\n";
	count = 0;
	for (auto& row : CSVRange(in)) {
		if (count == 10) {
			out_csv << row[0] << "," << row[1] << "," << row[9] << "," << row[10] << "," << row[14]<< "," << row[15] << std::endl;
			count = 1;
		} else ++count;
	}
	out_csv.close();
	in.close();
	in.open("p1.csv", std::ios_base::in);
	out_csv.open("p3/p3_table.csv", std::ios_base::out);
	out_csv << "LaunchNum,AbsError(write),RelError(write),AbsError(read),RelError(read)\n";
	count = 0;
	for (auto& row : CSVRange(in)) {
		if (count < 21) {
			++count;
			continue;
		} else if (count < 31) {
			out_csv << row[4] << "," << row[9] << "," << row[10] << "," << row[14]<< "," << row[15] << std::endl;
			++count;
		} else break;
	}
	out_csv.close();
	in.close();
	// in.open("p2.csv", std::ios_base::in);
	// out_csv.open("p3/p4_table.csv", std::ios_base::out);
	// out_csv << "MemoryType,ElementType,WriteBandwidth,ReadBandwidth\n";
	// count = 0;
	// int temp = 0, temp1 = 0;
	// int a; long b; long long c = 1e12;
	// for (auto& row : CSVRange(in)) {
	// 	if (count < 241) {
	// 		++count;
	// 		continue;
	// 	} else if (temp == 9) {
	// 		if (row[2] == "i")
	// 		out_csv << row[0] << "," << sizeof(a) << "," << row[8] << "," << row[13] << std::endl;
	// 		if (row[2] == "l")
	// 		out_csv << row[0] << "," << sizeof(b) << "," << row[8] << "," << row[13] << std::endl;
	// 		if (row[2] == "x")
	// 		out_csv << row[0] << "," << sizeof(c) << "," << row[8] << "," << row[13] << std::endl;
	// 		temp = 0;
	// 	} else ++temp;
	// }
	// out_csv.close();
	// in.close();
	kek("p1.csv", "data/p1_write_ram.data", 21, 8);
	kek("p1.csv", "data/p1_read_ram.data", 21, 13);
	kek("p1.csv", "data/p1_write_hdd.data", 11, 8);
	kek("p1.csv", "data/p1_read_hdd.data", 11, 13);
	kek("p1.csv", "data/p1_write_flash.data", 1, 8);
	kek("p1.csv", "data/p1_read_flash.data", 1, 13);
	kek("p1.csv", "data/p2_write_ram_abs_err.data", 21, 9);
	kek("p1.csv", "data/p2_write_ram_rel_err.data", 21, 10);
	kek("p1.csv", "data/p2_read_ram_abs_err.data", 21, 14);
	kek("p1.csv", "data/p2_read_ram_rel_err.data", 21, 15);
	kek("p1.csv", "data/p2_write_hdd_abs_err.data", 11, 9);
	kek("p1.csv", "data/p2_write_hdd_rel_err.data", 11, 10);
	kek("p1.csv", "data/p2_read_hdd_abs_err.data", 11, 14);
	kek("p1.csv", "data/p2_read_hdd_rel_err.data", 11, 15);
	kek("p1.csv", "data/p2_write_flash_abs_err.data", 1, 9);
	kek("p1.csv", "data/p2_write_flash_rel_err.data", 1, 10);
	kek("p1.csv", "data/p2_read_flash_abs_err.data", 1, 14);
	kek("p1.csv", "data/p2_read_flash_rel_err.data", 1, 15);
	eke("p3/p3_table.csv", "data/p3_write_flash_abs_err.data", 1);
	eke("p3/p3_table.csv", "data/p3_write_flash_rel_err.data", 2);
	eke("p3/p3_table.csv", "data/p3_read_flash_abs_err.data", 3);
	eke("p3/p3_table.csv", "data/p3_read_flash_rel_err.data", 4);
	kek1("p3/p4_table.csv", "data/p4_write_hdd.data", 0, 2);
	kek1("p3/p4_table.csv", "data/p4_read_hdd.data", 0, 3);
	kek1("p3/p4_table.csv", "data/p4_write_flash.data", -1, 2);
	kek1("p3/p4_table.csv", "data/p4_read_flash.data", -1, 3);
	return 0;
}