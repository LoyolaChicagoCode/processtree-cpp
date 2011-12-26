#include <iostream>
#include <string>

using std::string;

void print_space() { std::cout << ' '; }

void print_process_info(const int i, const std::string cmd) {
	std::cout << i << ": " << cmd << '\n';
}

void set_stdout_buffer(char* const obuf, const int size) {
	std::cout.rdbuf()->pubsetbuf(obuf, size);
}

bool read_line(char* const buf, const int size) {
	return std::cin.getline(buf, size);
}

void flush_stdout() { std::cout.flush(); }
