#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cin;

void print_spaces(const unsigned int n) { cout << string(n, ' '); }

void print_process_info(const unsigned int i, const string& cmd) {
	cout << i << ": " << cmd << '\n';
}

void init_io() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr);
}

bool read_line(char* const buf, const size_t size) {
	return cin.getline(buf, size);
}

void flush_stdout() { cout.flush(); }
