/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PARSING_H_
#define PARSING_H_

#include <cstdlib> // atoi
#include <cstring> // strtok
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // max

#include "process.h"

using namespace std;

typedef vector<string>::iterator IT;

const char* const TERM = " \t\n\r";
const char* const EOL = "\n\r";

struct Format_Info {
	size_t first, second, cmd;
	bool pidFirst;
};

size_t find_header_field(vector<string>& tokens, const char* const name, bool optional = false) {
	const IT first = tokens.begin(), last = tokens.end();
	const IT it = find(first, last, name);
	if (! optional && it == last) {
		cerr << "required header field " << name << " missing!" << endl;
		exit(1);
	}
	return it - first;
}

Format_Info::Format_Info(char* const header) {
	const char* tok = strtok(header, TERM);
	vector<string> tokens;
	while (tok != NULL) {
		tokens.push_back(tok);
		tok = strtok(NULL, TERM);
	}
	Format_Info& fi = *this;
	const int pid = find_header_field(tokens, "PID");
	const int ppid = find_header_field(tokens, "PPID");
	fi.cmd = find_header_field(tokens, "CMD", true);
	if (fi.cmd >= tokens.size())
		fi.cmd = find_header_field(tokens, "COMMAND");
	if (fi.cmd < max(pid, ppid)) {
		cerr << "CMD header must follow PID and PPID!" << endl;
		exit(1);
	}
	if (pid < ppid) {
		fi.first = pid;
		fi.second = ppid;
		fi.pidFirst = true;
	} else {
		fi.first = ppid;
		fi.second = pid;
		fi.pidFirst = false;
	}
}

void Format_Info::parse_process(process& p, char* const line) {
	Format_Info& fi = *this;
	const char* tok = strtok(line, TERM);
	for (int i = 0; i < fi.first; i++) tok = strtok(NULL, TERM);
	if (fi.pidFirst) p.pid = atoi(tok); else p.ppid = atoi(tok);
	for (int i = fi.first; i < fi.second; i++) tok = strtok(NULL, TERM);
	if (fi.pidFirst) p.ppid = atoi(tok); else p.pid = atoi(tok);
	for (int i = fi.second; i < fi.cmd - 1; i++) tok = strtok(NULL, TERM);
	p.cmd = strtok(NULL, EOL);
	// strip trailing newline if present
	size_t l = p.cmd.find('\n');
	if (l != string::npos) p.cmd = p.cmd.substr(0, int(l));
	l = p.cmd.find('\r');
	if (l != string::npos) p.cmd = p.cmd.substr(0, int(l));
}

#endif /* PARSING_H_ */
