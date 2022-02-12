/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#include <cstring> // strtok
#include <string>
#include <vector>
#include <algorithm> // find, max
#include <spdlog/spdlog.h>
#include <scn/scn.h>

#include "parsing.h"
#include "process.h"

using std::min;
using std::max;
using std::string;
using std::vector;

void report_missing_header_field(const string& name) {
    spdlog::error("required header field {} missing!", name);
    throw 1;
}

size_t find_header_field(vector<string>& tokens, const string& name) {
    const vector<string>::iterator first = tokens.begin(), last = tokens.end();
    const vector<string>::iterator it = find(first, last, name);
    return it < last ? it - first : string::npos;
}

process_parser::process_parser(const string& header) {
    spdlog::debug("header string {}", header);
    vector<string> tokens;
    scn::scan_list(header, tokens);
    pid = find_header_field(tokens, "PID");
    if (pid == string::npos) report_missing_header_field("PID");
    ppid = find_header_field(tokens, "PPID");
    if (ppid == string::npos) report_missing_header_field("PPID");
    auto pos_cmd = find_header_field(tokens, "CMD");
    auto pos_command = find_header_field(tokens, "COMMAND");
    auto pos = min(pos_cmd, pos_command);
    cmd = min(header.find("CMD"), header.find("COMMAND"));
    spdlog::debug("header columns {} {} {} {} {}", pid, ppid, pos, cmd, tokens.size());
    if (pos < tokens.size() - 1) {
        spdlog::error("required header field CMD or COMMAND missing or not last!");
        throw 1;
    }
}

process process_parser::parse(const string& line) const {
    vector<string> tokens;
    process proc;
    scn::scan_list(line.substr(0, cmd), tokens);
    proc.pid = std::stoi(tokens[pid]);
    proc.ppid = std::stoi(tokens[ppid]);
    proc.cmd = line.substr(cmd);
    return proc;
}
