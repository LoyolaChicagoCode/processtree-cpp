/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#include <cstdlib> // atoi
#include <cstring> // strtok
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm> // find, max

#include "parsing.h"
#include "process.h"

using std::find;
using std::min;
using std::max;
using std::string;
using std::vector;

const char* const TERM = " \t\n\r";
const char* const EOL = "\n\r";

size_t find_header_field(vector<string>& tokens, const char* const name) {
	const vector<string>::iterator first = tokens.begin(), last = tokens.end();
	const vector<string>::iterator it = find(first, last, name);
	return it < last ? it - first : string::npos;
}

void report_missing_header_field(const char* const name) {
	fprintf(stderr, "required header field %s missing!\n", name);
	exit(1);
}

process_parser::process_parser(char* const header) {
	const char* tok = strtok(header, TERM);
	vector<string> tokens;
	while (tok != NULL) {
		tokens.push_back(tok);
		tok = strtok(NULL, TERM);
	}
	const size_t pid = find_header_field(tokens, "PID");
	if (pid == string::npos) report_missing_header_field("PID");
	const size_t ppid = find_header_field(tokens, "PPID");
	if (ppid == string::npos) report_missing_header_field("PPID");
	cmd = min(find_header_field(tokens, "CMD"), find_header_field(tokens, "COMMAND"));
	if (cmd < max(pid, ppid)) {
		fputs("required header field CMD or COMMAND missing or not last!", stderr);
		exit(1);
	}
	if (pid < ppid) {
		first = pid; second = ppid; pidFirst = true;
	} else {
		first = ppid; second = pid; pidFirst = false;
	}
}

void process_parser::parse(process& p, char* const line) const {
	const char* tok = strtok(line, TERM);
	for (size_t i = 0; i < first; i++) tok = strtok(NULL, TERM);
	if (pidFirst) p.pid = atoi(tok); else p.ppid = atoi(tok);
	for (size_t i = first; i < second; i++) tok = strtok(NULL, TERM);
	if (pidFirst) p.ppid = atoi(tok); else p.pid = atoi(tok);
	for (size_t i = second; i < cmd - 1; i++) tok = strtok(NULL, TERM);
	p.cmd = strtok(NULL, EOL);
}
