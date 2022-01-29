#include <vector>
#include <unordered_map>
#include <spdlog/spdlog.h>

#include "io.h"
#include "process.h"
#include "parsing.h"

using std::unordered_map;
using std::vector;
using std::pair;
using std::string;

const auto LINE_BUF_SIZE = 16384; // commands on MacOS can be really long!
const auto IO_BUF_SIZE = 8192;

void print_tree(unordered_map<int, string>& m, unordered_map<int, vector<int> >& t, int i, int l) {
    // indent, then print current process
    for (auto k = 0; k < l; k++)
        print_space();
    print_process_info(i, m[i]);
    // print children indented by one more level
    for (auto e = t[i].begin(); e != t[i].end(); e++)
        print_tree(m, t, *e, l + 1);
}

int main(int argc, char* argv[]) {
    char ibuf[LINE_BUF_SIZE];
    char obuf[IO_BUF_SIZE];
    unordered_map<int, string> m;
    unordered_map<int, vector<int> > t;

    // analyze header line
    set_stdout_buffer(obuf, IO_BUF_SIZE);
    read_line(ibuf, LINE_BUF_SIZE);
    process_parser parser(ibuf);

    // read lines, parse to process object, and insert into table
    spdlog::debug("starting to parse processes");
    process proc;
    auto count = 0;
    while (read_line(ibuf, LINE_BUF_SIZE)) {
        spdlog::debug("{} characters in the current input line", strlen(ibuf));
        count ++;
        proc = parser.parse(ibuf);
        m.insert(pair<int, string>(proc.pid, proc.cmd));
        t[proc.ppid].push_back(proc.pid);
    }
    spdlog::debug("{} processes parsed", count);

    // print as tree
    for (auto e = t[0].begin(); e != t[0].end(); e++)
        print_tree(m, t, *e, 0);
    flush_stdout();
}
