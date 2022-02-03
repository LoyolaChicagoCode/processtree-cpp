#include <cstdio>
#include <string>

using std::string;

void print_spaces(const unsigned int n) { puts(string(n, ' ').c_str()); }

void print_process_info(const unsigned int i, const std::string& cmd) {
	printf("%d: ", i);
	puts(cmd.c_str());
}

void init_io() { }

bool read_line(char* const buf, const size_t size) {
	return fgets(buf, size, stdin);
}

void flush_stdout() { fflush(stdout); }
