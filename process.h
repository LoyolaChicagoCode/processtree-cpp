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
	string uid;
	unsigned int pid, ppid, c;
	string stime, tty, time, cmd;
	process();
	process(char* line);
	friend std::ostream& operator<<(std::ostream& str, process const& data);
};

process::process() { }

process::process(char* line) {
	const char* tok;
	tok = strtok(line, " ");  uid = tok;
	tok = strtok(NULL, " "); pid = atoi(tok);
	tok = strtok(NULL, " "); ppid = atoi(tok);
	tok = strtok(NULL, " "); c = atoi(tok);
	tok = strtok(NULL, " "); stime = tok;
	tok = strtok(NULL, " "); tty = tok;
	tok = strtok(NULL, " "); time = tok;
	tok = strtok(NULL, " "); cmd = tok;
	// strip trailing newline if present
	const size_t l = cmd.find('\n');
	if (l != string::npos)
		cmd = cmd.substr(0, int(l));
}

ostream& operator<<(std::ostream& str, process const& data) {
	str << "Proc("
		<< data.uid << ", "
		<< data.pid << ", "
		<< data.ppid << ", "
		<< data.c << ", "
		<< data.stime << ", "
		<< data.tty << ", "
		<< data.time << ", "
		<< data.cmd.substr(0, 20)
		<< ")";
	return str;
}

#endif /* PROCESS_H_ */
