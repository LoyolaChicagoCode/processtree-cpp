#include <iostream>
#include <vector>
#include <unordered_map>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "process.h"
#include "parsing.h"

using std::string;
using std::pair;
using std::unordered_map;
using std::vector;
using std::cin;
using std::getline;

typedef vector<unsigned int> p_list;
typedef unordered_map<unsigned int, string> cmd_map;
typedef unordered_map<unsigned int, vector<unsigned int> > ppid_map;

void print_tree(cmd_map& m, ppid_map& t, unsigned int i, unsigned int l) {
    // indent, then print current process
    fmt::print("{} {}: {}\n", string(l, ' '), i, m[i]);
    // print children indented by one more level
    for (auto e = t[i].begin(); e != t[i].end(); e++)
        print_tree(m, t, *e, l + 1);
}

int main(int argc, char* argv[]) {
    cmd_map m;
    ppid_map t;

    spdlog::set_level(spdlog::level::warn);

    string line;
    getline(cin, line);
    process_parser parser(line);

    // read lines, parse to process object, and insert into table
    spdlog::debug("starting to parse processes");
    auto count = 0;
    while (getline(cin, line)) {
        spdlog::debug("input line {}", line);
        count ++;
        auto proc = parser.parse(line);
        spdlog::debug("process {} {} {}", proc.pid, proc.ppid, proc.cmd);
        m.insert(pair<unsigned int, string&>(proc.pid, proc.cmd));
        t[proc.ppid].push_back(proc.pid);
    }
    spdlog::debug("{} processes parsed", count);

    // print as tree
    for (auto e = t[0].begin(); e != t[0].end(); e++)
        print_tree(m, t, *e, 0);
}
