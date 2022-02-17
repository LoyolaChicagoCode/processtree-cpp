#include <iostream>
#include <vector>
#include <forward_list>
#include <chrono>
#include <unordered_map>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <CLI/CLI.hpp>

#include "parsing.h"

using std::string;
using std::pair;
using std::unordered_map;
using std::vector;
using std::forward_list;
using std::cin;
using std::getline;

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

typedef unordered_map<unsigned int, const string> cmd_map;
typedef unordered_map<unsigned int, vector<unsigned int> > ppid_map;

// can use const pair with clang++ but not g++
typedef vector<pair<const string, const steady_clock::time_point> > ts_vector;

void read_input(string& header, forward_list<string>& lines) {
    getline(cin, header);
    string line;
    auto count = 0;
    while (getline(cin, line)) {
        spdlog::debug("input line {}", line);
        lines.push_front(line);
        count++;
    }
    spdlog::debug("{} lines read", count);
}

void parse_lines(const string& header, const forward_list<string>& lines, forward_list<process>& processes) {
    process_parser parser(header);
    auto count = 0;
    spdlog::debug("starting to parse processes");
    for (auto it = lines.begin(); it != lines.end(); it++) {
        processes.push_front(parser.parse(*it));
        count++;
    }
    spdlog::debug("{} processes parsed", count);
}

void insert_processes(const forward_list<process>& processes, cmd_map& m, ppid_map& t) {
    auto count = 0;
    spdlog::debug("starting to insert processes into table");
    for (auto it = processes.begin(); it != processes.end(); it++) {
        const process& proc = *it;
        m.insert(pair<unsigned int, const string&>(proc.pid, proc.cmd));
        t[proc.ppid].push_back(proc.pid);
        count++;
    }
    spdlog::debug("{} processes inserted", count);
}

void print_tree(FILE* const dest, const cmd_map& m, const ppid_map& t, const unsigned int i, const unsigned int l) {
    // indent, then print current process
    fmt::print(dest,"{} {}: {}\n", string(l, ' '), i, m.at(i));
    // return if current process is a leaf
    auto it = t.find(i);
    if (it == t.end()) return;
    // print children indented by one more level
    for (auto e = it->second.begin(); e != it->second.end(); e++)
        print_tree(dest, m, t, *e, l + 1);
}

void print_forest(FILE* const dest, const cmd_map& m, const ppid_map& t) {
    for (auto e = t.at(0).begin(); e != t.at(0).end(); e++)
        print_tree(stdout, m, t, *e, 0);
}

void mark_time(ts_vector& timestamps, const string& label) {
    timestamps.push_back(pair<const string&, const steady_clock::time_point&>(label, steady_clock::now()));
}

void print_timestamps(const ts_vector& timestamps) {
    for (auto t = timestamps.begin() + 1; t != timestamps.end(); t++)
        spdlog::info("{}: {} ms", t->first, duration_cast<milliseconds>(t->second - (t - 1)->second).count());
    auto start = timestamps.front().second;
    auto stop = timestamps.back().second;
    spdlog::info("TOTAL time: {} ms", duration_cast<milliseconds>(stop - start).count());
}

// FIXME make strongly typed by adding back class keyword
enum Input : int { stdio, scn_getline, cin_getline, std_getline };

int main(const int argc, const char* const argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_default_logger(spdlog::stderr_color_st("arbitrary"));
    spdlog::set_default_logger(spdlog::stderr_color_st(""));

    CLI::App app{"processtree: show process hierarchy as a tree"};

    // https://github.com/CLIUtils/CLI11/blob/main/examples/enum.cpp

    Input input {Input:Input::std_getline};
    app.add_option("-i,--input", input, "Input method");

    CLI11_PARSE(app, argc, argv);
    spdlog::info("input method {}", input);

    ts_vector timestamps;

    string header;
    forward_list<string> lines;
    forward_list<process> processes;
    cmd_map m;
    ppid_map t;

    mark_time(timestamps, "Start time");
    read_input(header, lines);
    mark_time(timestamps, "Input time");
    parse_lines(header, lines, processes);
    mark_time(timestamps, "Parsing time");
    insert_processes(processes, m, t);
    mark_time(timestamps, "Insertion time");
    print_forest(stdout, m, t);
    mark_time(timestamps, "Output time");
    print_timestamps(timestamps);
}
