#include <istream>
#include <vector>
#include <forward_list>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

#include <fmt/format.h>
#include <scn/scn.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "processtree.h"

using std::string, std::string_view;
using std::pair, std::unordered_map, std::vector, std::forward_list;

void read_input(std::istream& input, string& header, forward_list<string>& lines) {
    getline(input, header);
    string line;
    auto count = 0;
    while (getline(input, line)) {
        lines.push_front(line);
        count++;
    }
    spdlog::debug("{} lines read", count);
}

parser parse_header(const string& header) {
    spdlog::debug("header string {}", header);
    vector<string> tokens;
    scn::scan_list(header, tokens);
    const auto first = tokens.begin();
    const auto last = tokens.end();
    const auto pid = find(first, last, "PID") - first;
    if (pid >= tokens.size()) throw std::runtime_error("required header field PID missing!");
    const auto ppid = find(first, last, "PPID") - first;
    if (ppid >= tokens.size()) throw std::runtime_error("required header field PPID missing!");
    const auto cmd = std::min(header.find("CMD"), header.find("COMMAND"));
    const auto& rightmost = tokens[tokens.size() - 1];
    if (rightmost != "CMD" && rightmost != "COMMAND") throw std::runtime_error("required header field CMD or COMMAND missing or not last!");
    spdlog::debug("header columns {} {} {} {}", pid, ppid, cmd, tokens.size());
    return { pid, ppid, cmd };
}

void parse_lines(const parser& parser, const forward_list<string>& lines, forward_list<process>& processes) {
    vector<string> tokens;
    auto count = 0;
    for (const auto& line: lines) {
        tokens.clear();
        scn::scan_list(line.substr(0, parser.cmd), tokens);
        process proc{
                std::stoi(tokens[parser.pid]),
                std::stoi(tokens[parser.ppid]),
                line.substr(parser.cmd)
        };
        processes.push_front(proc);
        count ++;
    }
    spdlog::debug("{} processes parsed", count);
}

void insert_processes(const forward_list<process>& processes, cmd_map& m, ppid_map& t) {
    auto count = 0;
    spdlog::debug("starting to insert processes into table");
    for (const auto& proc: processes) {
        m.insert(pair(proc.pid, proc.cmd));
        t[proc.ppid].push_back(proc.pid);
        count++;
    }
    spdlog::debug("{} processes inserted", count);
}

// print children indented by one more level
void print_children(FILE* const dest, const cmd_map& m, const ppid_map& t, const vector<unsigned int>& cs, const unsigned int l) {
    for (const auto k: cs)
        print_tree(dest, m, t, k, l);
}

void print_tree(FILE* const dest, const cmd_map& m, const ppid_map& t, const unsigned int i, const unsigned int l) {
    // indent, then print current process
    fmt::print(dest,"{}{}: {}\n", string(2 * l, ' '), i, m.at(i));
    // return if current process is a leaf
    const auto it = t.find(i);
    if (it == t.end()) return;
    print_children(dest, m, t, it->second, l + 1);
}

void print_forest(FILE* const dest, const cmd_map& m, const ppid_map& t) {
    print_children(dest, m, t, t.at(0), 0);
}
