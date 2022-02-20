#include <iostream>
#include <vector>
#include <forward_list>
#include <unordered_map>
#include <chrono>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <CLI/CLI.hpp>
#include <magic_enum.hpp>

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
    const process_parser parser(header);
    auto count = 0;
    spdlog::debug("starting to parse processes");
    for (const auto& line: lines) {
        processes.push_front(parser.parse(line));
        count++;
    };
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

void print_tree(FILE* const dest, const cmd_map& m, const ppid_map& t, const unsigned int i, const unsigned int l) {
    // indent, then print current process
    fmt::print(dest,"{} {}: {}\n", string(l, ' '), i, m.at(i));
    // return if current process is a leaf
    const auto it = t.find(i);
    if (it == t.end()) return;
    // print children indented by one more level
    for (const auto i: it->second)
        print_tree(dest, m, t,i, l + 1);
}

void print_forest(FILE* const dest, const cmd_map& m, const ppid_map& t) {
    for (const auto i: t.at(0))
        print_tree(dest, m, t, i, 0);
}

void mark_time(ts_vector& timestamps, const string& label) {
    timestamps.push_back(pair(label, steady_clock::now()));
}

void print_timestamps(const ts_vector& timestamps) {
    for (auto t = timestamps.begin() + 1; t != timestamps.end(); t++) {
        const auto dur = duration_cast<milliseconds>(t->second - (t - 1)->second).count();
        spdlog::info("{}: {} ms", t->first, dur);
    }
    const auto& start = timestamps.front().second;
    const auto& stop = timestamps.back().second;
    spdlog::info("TOTAL time: {} ms", duration_cast<milliseconds>(stop - start).count());
}

// map from strings to corresponding enum values for arg parsing
// TODO move to separate util source
// TODO magic_enum PR
template <typename E> unordered_map<string, E> enum_rentries() {
    const auto& entries = magic_enum::enum_entries<E>();
    unordered_map<string, E> rentries;
    for (const auto& e: entries)
        rentries.insert(pair(e.second, e.first));
    return rentries;
}

enum class Input : int { stdio, scn, cin, std };

int main(const int argc, const char* const argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_default_logger(spdlog::stderr_color_st("arbitrary"));
    spdlog::set_default_logger(spdlog::stderr_color_st(""));

    CLI::App app{"processtree: show process hierarchy as a tree"};

    // https://github.com/CLIUtils/CLI11/blob/main/examples/enum.cpp
    Input input {Input::std};
    app.add_option("-i,--input", input, "Input method")
        ->transform(CLI::CheckedTransformer(enum_rentries<Input>(), CLI::ignore_case));

    CLI11_PARSE(app, argc, argv);
    spdlog::info("input method {}", magic_enum::enum_name<Input>(input));

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
