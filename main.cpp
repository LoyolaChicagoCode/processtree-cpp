#include <iostream>
#include <vector>
#include <unordered_map>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "parsing.h"

using std::string;
using std::pair;
using std::unordered_map;
using std::vector;
using std::cin;
using std::getline;

typedef unordered_map<unsigned int, string> cmd_map;
typedef unordered_map<unsigned int, vector<unsigned int> > ppid_map;

void print_tree(cmd_map& m, ppid_map& t, const unsigned int i, const unsigned int l) {
    // indent, then print current process
    fmt::print("{} {}: {}\n", string(l, ' '), i, m.at(i));
    // return if current process is a leaf
    if (t.count(i) == 0) return;
    // print children indented by one more level
    auto children = t.at(i);
    for (auto e = children.begin(); e != children.end(); e++)
        print_tree(m, t, *e, l + 1);
}

int main(const int argc, const char* const argv[]) {
    spdlog::set_level(spdlog::level::info);

    cmd_map m;
    ppid_map t;
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
    for (auto e = t.at(0).begin(); e != t.at(0).end(); e++)
        print_tree(m, t, *e, 0);
}
