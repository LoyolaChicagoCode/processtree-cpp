#include <vector>
#include <forward_list>
#include <unordered_map>
#include <chrono>
#include <stdexcept>

#include <CLI/CLI.hpp>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "processtree.h"

using std::string_view;
using std::pair;

// can use const pair with clang++ but not g++
typedef vector<pair<const string, const std::chrono::steady_clock::time_point> > ts_vector;

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

int main(const int argc, const char* const argv[]) {
    spdlog::set_level(spdlog::level::info);
    spdlog::set_default_logger(spdlog::stderr_color_st("arbitrary"));
    spdlog::set_default_logger(spdlog::stderr_color_st(""));

    CLI::App app{"processtree: show process hierarchy as a tree"};

    // https://github.com/CLIUtils/CLI11/blob/main/examples/enum.cpp
    enum class Input : int { stdio, scn, cin, std };
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
