/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdlib.h> // atoi
#include <string.h> // strtok
#include <iostream>
#include <string>
#include <algorithm> // max

using namespace std;

struct process {
	unsigned int pid, ppid;
	string cmd;
	process();
	process(char* line, int i_pid, int i_ppid, int i_cmd);
	friend std::ostream& operator<<(std::ostream& str, process const& data);
};

process::process() { }

process::process(char* line, int i_pid, int i_ppid, int i_cmd) {
	const char* tok;
	tok = strtok(NULL, " "); pid = atoi(tok);
	tok = strtok(NULL, " "); ppid = atoi(tok);
	tok = strtok(NULL, " "); cmd = tok;
	// strip trailing newline if present
	const size_t l = cmd.find('\n');
	if (l != string::npos)
		cmd = cmd.substr(0, int(l));
}

ostream& operator<<(std::ostream& str, process const& data) {
	return str << data.pid << ": " << data.cmd << ")";
}

#endif /* PROCESS_H_ */
