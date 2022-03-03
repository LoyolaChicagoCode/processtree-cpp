#include <vector>
#include <forward_list>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <stdexcept>

#include <CLI/CLI.hpp>
#include <magic_enum.hpp>
#include <fmt/format.h>
#include <scn/scn.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using std::string, std::string_view;
using std::pair, std::unordered_map, std::vector, std::forward_list;

typedef unordered_map<unsigned int, const string> cmd_map;
typedef unordered_map<unsigned int, vector<unsigned int> > ppid_map;

// parsing info from header row of ps command
struct parser {
    long pid, ppid;    // field numbers of PID and PPID in header row
    unsigned long cmd; // column of CMD or COMMAND field in header row
};

// representation of a single process
struct process {
    long pid, ppid;
    string cmd;
};

// can use const pair with clang++ but not g++
typedef vector<pair<const string, const std::chrono::steady_clock::time_point> > ts_vector;

void read_input(std::istream& input, string& header, forward_list<string>& lines) {
    std::getline(input, header);
    string line;
    auto count = 0;
    while (std::getline(input, line)) {
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

void mark_time(ts_vector& timestamps, const string_view label) {
    timestamps.push_back(pair(label.data(), std::chrono::steady_clock::now()));
}

void print_timestamps(const ts_vector& timestamps) {
    using std::chrono::milliseconds;
    using std::chrono::duration_cast;
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
    read_input(std::cin, header, lines);
    mark_time(timestamps, "Input time");
    auto parser = parse_header(header);
    parse_lines(parser, lines, processes);
    mark_time(timestamps, "Parsing time");
    insert_processes(processes, m, t);
    mark_time(timestamps, "Insertion time");
    print_forest(stdout, m, t);
    mark_time(timestamps, "Output time");
    print_timestamps(timestamps);
}
