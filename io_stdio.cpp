#include <cstdio>
#include <string>

using std::string;

void print_space() { putchar(' '); }

void print_process_info(const int i, const std::string cmd) {
    printf("%d: ", i);
    puts(cmd.c_str());
}

void set_stdout_buffer(char* const obuf, const int size) {
    setbuf(stdout, obuf);
}

bool read_line(char* const buf, const int size) {
    return fgets(buf, size, stdin);
}

void flush_stdout() { fflush(stdout); }
