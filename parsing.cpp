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

#include "parsing.h"
#include "process.h"

using std::find;
using std::max;
using std::stoi;
using std::string;
using std::vector;

const auto TERM = " \t\n\r";
const auto EOL = "\n\r";

int find_header_field(vector<string>& tokens, const char* const name) {
    const auto first = tokens.begin(), last = tokens.end();
    const auto it = find(first, last, name);
    return it < last ? it - first : -1;
}

void report_missing_header_field(const char* const name) {
    spdlog::error("required header field {} missing!", name);
    exit(1);
}

process_parser::process_parser(char* const header) {
    auto tok = strtok(header, TERM);
    vector<string> tokens;
    while (tok != NULL) {
        tokens.push_back(tok);
        tok = strtok(NULL, TERM);
    }
    const auto pid = find_header_field(tokens, "PID");
    if (pid < 0) report_missing_header_field("PID");
    const auto ppid = find_header_field(tokens, "PPID");
    if (ppid < 0) report_missing_header_field("PPID");
    cmd = max(find_header_field(tokens, "CMD"), find_header_field(tokens, "COMMAND"));
    if (cmd < max(pid, ppid)) {
        spdlog::error("required header field CMD or COMMAND missing or not last!");
        exit(1);
    }
    if (pid < ppid) {
        first = pid; second = ppid; pidFirst = true;
    } else {
        first = ppid; second = pid; pidFirst = false;
    }
}

process process_parser::parse(char* const line) const {
    process p;
    auto tok = strtok(line, TERM);
    for (auto i = 0; i < first; i++) tok = strtok(NULL, TERM);
    if (pidFirst) p.pid = std::stoi(tok); else p.ppid = stoi(tok);
    for (auto i = first; i < second; i++) tok = strtok(NULL, TERM);
    if (pidFirst) p.ppid = stoi(tok); else p.pid = stoi(tok);
    for (auto i = second; i < cmd - 1; i++) tok = strtok(NULL, TERM);
    p.cmd = strtok(NULL, EOL);
    // strip trailing newline if present
    auto l = p.cmd.find('\n');
    if (l != string::npos) p.cmd = p.cmd.substr(0, int(l));
    l = p.cmd.find('\r');
    if (l != string::npos) p.cmd = p.cmd.substr(0, int(l));
    return p;
}
