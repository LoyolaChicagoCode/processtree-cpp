#include <forward_list>

#include <CLI/CLI.hpp>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "processtree.h"
#include "util.h"

using std::string;
using std::forward_list;

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
